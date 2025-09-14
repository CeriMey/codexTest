#include "st0601.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <limits>
#include <vector>

namespace misb {
namespace st0601 {

namespace detail {

template <typename T>
constexpr T clamp(T v, T lo, T hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

struct Codec {
    std::function<std::vector<uint8_t>(double)> enc;
    std::function<double(const std::vector<uint8_t>&)> dec;
};

using misb::pack_be;
using misb::unpack_be;

constexpr UL make_ul(uint8_t tag) {
    return make_st_ul(ST_ID, tag);
}

inline Codec codec_u16_linear(double min, double max) {
    return Codec{
        [=](double v) {
            const double lo = std::min(min, max);
            const double hi = std::max(min, max);
            const double clamped = clamp(v, lo, hi);
            const double span = hi - lo;
            const uint16_t raw = static_cast<uint16_t>(std::lround((clamped - lo) * (65535.0 / span)));
            return pack_be<uint16_t>(raw);
        },
        [=](const std::vector<uint8_t>& bytes) {
            uint16_t raw{};
            if (!unpack_be<uint16_t>(bytes, raw)) return std::numeric_limits<double>::quiet_NaN();
            const double lo = std::min(min, max);
            const double hi = std::max(min, max);
            const double span = hi - lo;
            return lo + (static_cast<double>(raw) * (span / 65535.0));
        }
    };
}

inline Codec codec_u32_linear(double min, double max) {
    return Codec{
        [=](double v) {
            const double lo = std::min(min, max);
            const double hi = std::max(min, max);
            const double clamped = clamp(v, lo, hi);
            const double span = hi - lo;
            const uint32_t raw = static_cast<uint32_t>(std::llround((clamped - lo) * (4294967295.0 / span)));
            return pack_be<uint32_t>(raw);
        },
        [=](const std::vector<uint8_t>& bytes) {
            uint32_t raw{};
            if (!unpack_be<uint32_t>(bytes, raw)) return std::numeric_limits<double>::quiet_NaN();
            const double lo = std::min(min, max);
            const double hi = std::max(min, max);
            const double span = hi - lo;
            return lo + (static_cast<double>(raw) * (span / 4294967295.0));
        }
    };
}

inline Codec codec_s16_symmetric(double A) {
    return Codec{
        [=](double v) {
            if (std::isnan(v)) return pack_be<int16_t>(static_cast<int16_t>(0x8000));
            const double clamped = clamp(v, -A, +A);
            const double scale = 32767.0 / A;
            int16_t raw = static_cast<int16_t>(std::lround(clamped * scale));
            if (raw == std::numeric_limits<int16_t>::min()) raw = -32767;
            return pack_be<int16_t>(raw);
        },
        [=](const std::vector<uint8_t>& bytes) {
            int16_t raw{};
            if (!unpack_be<int16_t>(bytes, raw)) return std::numeric_limits<double>::quiet_NaN();
            if (raw == std::numeric_limits<int16_t>::min()) return std::numeric_limits<double>::quiet_NaN();
            return static_cast<double>(raw) * (A / 32767.0);
        }
    };
}

inline Codec codec_s32_symmetric(double A) {
    return Codec{
        [=](double v) {
            if (std::isnan(v)) return pack_be<int32_t>(static_cast<int32_t>(0x80000000));
            const double clamped = clamp(v, -A, +A);
            const double scale = 2147483647.0 / A;
            int32_t raw = static_cast<int32_t>(std::llround(clamped * scale));
            if (raw == std::numeric_limits<int32_t>::min()) raw = -2147483647;
            return pack_be<int32_t>(raw);
        },
        [=](const std::vector<uint8_t>& bytes) {
            int32_t raw{};
            if (!unpack_be<int32_t>(bytes, raw)) return std::numeric_limits<double>::quiet_NaN();
            if (raw == std::numeric_limits<int32_t>::min()) return std::numeric_limits<double>::quiet_NaN();
            return static_cast<double>(raw) * (A / 2147483647.0);
        }
    };
}

inline Codec codec_u8_direct() {
    return Codec{
        [](double v) {
            const uint8_t raw = static_cast<uint8_t>(clamp(v, 0.0, 255.0));
            return std::vector<uint8_t>{ raw };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return std::numeric_limits<double>::quiet_NaN();
            return static_cast<double>(bytes[0]);
        }
    };
}

inline Codec codec_u8_linear(double min, double max) {
    return Codec{
        [=](double v) {
            const double lo = std::min(min, max);
            const double hi = std::max(min, max);
            const double clamped = clamp(v, lo, hi);
            const double span = hi - lo;
            const uint8_t raw = static_cast<uint8_t>(std::lround((clamped - lo) * (255.0 / span)));
            return std::vector<uint8_t>{ raw };
        },
        [=](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return std::numeric_limits<double>::quiet_NaN();
            const double lo = std::min(min, max);
            const double hi = std::max(min, max);
            const double span = hi - lo;
            return lo + (static_cast<double>(bytes[0]) * (span / 255.0));
        }
    };
}

inline Codec codec_u16_direct() {
    return Codec{
        [](double v) {
            const uint16_t raw = static_cast<uint16_t>(clamp(v, 0.0, 65535.0));
            return pack_be<uint16_t>(raw);
        },
        [](const std::vector<uint8_t>& bytes) {
            uint16_t raw{};
            if (!unpack_be<uint16_t>(bytes, raw)) return std::numeric_limits<double>::quiet_NaN();
            return static_cast<double>(raw);
        }
    };
}

inline Codec codec_u32_direct() {
    return Codec{
        [](double v) {
            const uint32_t raw = static_cast<uint32_t>(
                std::llround(clamp(v, 0.0, static_cast<double>(std::numeric_limits<uint32_t>::max()))));
            return pack_be<uint32_t>(raw);
        },
        [](const std::vector<uint8_t>& bytes) {
            uint32_t raw{};
            if (!unpack_be<uint32_t>(bytes, raw)) return std::numeric_limits<double>::quiet_NaN();
            return static_cast<double>(raw);
        }
    };
}

inline Codec codec_px_step2_u8() {
    return Codec{
        [](double px) {
            const double v = clamp(px, 0.0, 510.0);
            const uint8_t raw = static_cast<uint8_t>(std::lround(v / 2.0));
            return std::vector<uint8_t>{ raw };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return std::numeric_limits<double>::quiet_NaN();
            return static_cast<double>(bytes[0]) * 2.0;
        }
    };
}

inline Codec codec_time_u64() {
    return Codec{
        [](double usec) {
            const uint64_t raw = static_cast<uint64_t>(usec < 0.0 ? 0.0 : usec);
            return pack_be<uint64_t>(raw);
        },
        [](const std::vector<uint8_t>& bytes) {
            uint64_t raw{};
            if (!unpack_be<uint64_t>(bytes, raw)) return std::numeric_limits<double>::quiet_NaN();
            return static_cast<double>(raw);
        }
    };
}

inline Codec codec_angle_0_360_u16() { return codec_u16_linear(0.0, 360.0); }
inline Codec codec_angle_0_360_u32() { return codec_u32_linear(0.0, 360.0); }
inline Codec codec_angle_0_180_u16() { return codec_u16_linear(0.0, 180.0); }
inline Codec codec_lat_s32() { return codec_s32_symmetric(90.0); }
inline Codec codec_lon_s32() { return codec_s32_symmetric(180.0); }

} // namespace detail

#define REG(TAG_DEC, CODEC_EXPR) \
    reg.register_ul(detail::make_ul(static_cast<uint8_t>(TAG_DEC)), { (CODEC_EXPR).enc, (CODEC_EXPR).dec })

void register_st0601(KLVRegistry& reg) {
    using namespace detail;

    REG(2,  codec_time_u64());

    REG(5,  codec_angle_0_360_u16());
    REG(6,  codec_s16_symmetric(20.0));
    REG(7,  codec_s16_symmetric(50.0));
    REG(8,  codec_u8_direct());
    REG(9,  codec_u8_direct());
    REG(64, codec_angle_0_360_u16());
    REG(112, codec_angle_0_360_u16());
    REG(50, codec_s16_symmetric(20.0));
    REG(51, codec_s16_symmetric(180.0));
    REG(52, codec_s16_symmetric(20.0));

    REG(13, codec_lat_s32());
    REG(14, codec_lon_s32());
    REG(15, codec_u16_linear(-900.0, 19000.0));
    REG(16, codec_angle_0_180_u16());
    REG(17, codec_angle_0_180_u16());
    REG(18, codec_angle_0_360_u32());
    REG(19, codec_lon_s32());
    REG(20, codec_angle_0_360_u32());
    REG(75, codec_u16_linear(-900.0, 19000.0));
    REG(79, codec_s16_symmetric(327.0));
    REG(80, codec_s16_symmetric(327.0));

    REG(21, codec_u32_linear(0.0, 5000000.0));
    REG(22, codec_u16_linear(0.0, 10000.0));
    REG(40, codec_lat_s32());
    REG(41, codec_lon_s32());
    REG(42, codec_u16_linear(-900.0, 19000.0));
    REG(43, codec_px_step2_u8());
    REG(44, codec_px_step2_u8());
    REG(45, codec_u16_linear(0.0, 4095.0));
    REG(46, codec_u16_linear(0.0, 4095.0));

    REG(23, codec_lat_s32());
    REG(24, codec_lon_s32());
    REG(25, codec_u16_linear(-900.0, 19000.0));
    auto s16_offset = codec_s16_symmetric(0.15);
    REG(26, s16_offset); REG(27, s16_offset);
    REG(28, s16_offset); REG(29, s16_offset);
    REG(30, s16_offset); REG(31, s16_offset);
    REG(32, s16_offset); REG(33, s16_offset);
    REG(82, codec_lat_s32()); REG(83, codec_lon_s32());
    REG(84, codec_lat_s32()); REG(85, codec_lon_s32());
    REG(86, codec_lat_s32()); REG(87, codec_lon_s32());
    REG(88, codec_lat_s32()); REG(89, codec_lon_s32());

    REG(34, codec_u8_direct());
    REG(35, codec_angle_0_360_u16());
    REG(36, codec_u8_linear(0.0, 100.0));
    REG(37, codec_u16_linear(0.0, 5000.0));
    REG(38, codec_u16_linear(-900.0, 19000.0));
    reg.register_ul(make_ul(39), {
        [](double c) { return std::vector<uint8_t>{ static_cast<uint8_t>(static_cast<int8_t>(clamp(c, -128.0, 127.0))) }; },
        [](const std::vector<uint8_t>& bytes) -> double {
            if (bytes.size() != 1) return std::numeric_limits<double>::quiet_NaN();
            return static_cast<double>(static_cast<int8_t>(bytes[0]));
        }
    });
    REG(47, codec_u16_direct());
    REG(49, codec_u16_linear(0.0, 5000.0));
    REG(53, codec_u16_linear(0.0, 5000.0));
    REG(54, codec_u16_linear(-900.0, 19000.0));
    REG(55, codec_u8_linear(0.0, 100.0));

    REG(56, codec_u8_direct());
    REG(57, codec_u32_linear(0.0, 5000000.0));
    REG(58, codec_u16_linear(0.0, 10000.0));
    REG(60, codec_u16_direct());
    REG(61, codec_u8_direct());
    REG(62, codec_u16_direct());
    REG(63, codec_u8_direct());
    REG(65, codec_u8_direct());
    REG(67, codec_lat_s32());
    REG(68, codec_lon_s32());
    REG(69, codec_u16_linear(-900.0, 19000.0));
    REG(71, codec_angle_0_360_u16());
    REG(72, codec_time_u64());
    REG(76, codec_u16_linear(-900.0, 19000.0));
    REG(78, codec_u16_linear(-900.0, 19000.0));

    REG(90, codec_s32_symmetric(90.0));
    REG(91, codec_s32_symmetric(90.0));
    REG(92, codec_s32_symmetric(90.0));
    REG(93, codec_s32_symmetric(90.0));

    REG(94, codec_u16_direct());
    REG(95, codec_u16_direct());
    REG(96, codec_u16_direct());
    REG(97, codec_u16_direct());
    REG(98, codec_u16_direct());
    REG(99, codec_u16_direct());
    REG(100, codec_u16_direct());
    REG(101, codec_u16_direct());
    REG(102, codec_u16_direct());
    REG(103, codec_u16_direct());
    REG(104, codec_u16_direct());
    REG(105, codec_u16_direct());
    REG(107, codec_u16_direct());
    REG(108, codec_u16_direct());
    REG(109, codec_u16_direct());
    REG(110, codec_u16_direct());
    REG(111, codec_u16_direct());
    REG(113, codec_u16_direct());
    REG(114, codec_u16_direct());
    REG(115, codec_u16_direct());
    REG(116, codec_u16_direct());
    REG(117, codec_u16_direct());
    REG(118, codec_u16_direct());
    REG(119, codec_u16_direct());
    REG(120, codec_u16_direct());
    REG(121, codec_u16_direct());
    REG(122, codec_u16_direct());
    REG(123, codec_u16_direct());
    REG(124, codec_u16_direct());
    REG(125, codec_u16_direct());
    REG(126, codec_u16_direct());
    REG(127, codec_u16_direct());
    REG(128, codec_u16_direct());
    REG(129, codec_u16_direct());
    REG(130, codec_u16_direct());
    REG(131, codec_u16_direct());
    REG(132, codec_u16_direct());
    REG(133, codec_u16_direct());
    REG(134, codec_u16_direct());
    REG(135, codec_u16_direct());
    REG(136, codec_u16_direct());
    REG(137, codec_u16_direct());
    REG(138, codec_u16_direct());
    REG(139, codec_u16_direct());
    REG(140, codec_u16_direct());
    REG(141, codec_u16_direct());
    REG(142, codec_u16_direct());

    REG(81, codec_u16_direct());
    REG(77, codec_u8_direct());
}

#undef REG

} // namespace st0601
} // namespace misb

