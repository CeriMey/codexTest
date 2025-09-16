#include "st0903.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>

namespace misb {
namespace st0903 {

namespace detail {

inline double clamp(double v, double lo, double hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

inline std::vector<uint8_t> encode_uint(double value, size_t width) {
    if (width == 0 || width > 8) {
        return {};
    }
    const uint64_t max_value = width == 8 ? std::numeric_limits<uint64_t>::max()
                                          : ((uint64_t{1} << (width * 8)) - 1);
    const double clamped = clamp(value, 0.0, static_cast<double>(max_value));
    const uint64_t raw = static_cast<uint64_t>(std::llround(clamped));
    std::vector<uint8_t> bytes(width, 0u);
    for (size_t i = 0; i < width; ++i) {
        bytes[width - 1 - i] = static_cast<uint8_t>((raw >> (i * 8)) & 0xFFu);
    }
    return bytes;
}

inline double decode_uint(const std::vector<uint8_t>& bytes, size_t width) {
    if (bytes.size() != width || width == 0 || width > 8) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    uint64_t raw = 0;
    for (uint8_t b : bytes) {
        raw = (raw << 8) | b;
    }
    return static_cast<double>(raw);
}

inline std::vector<uint8_t> encode_probability(double value) {
    const double clamped = clamp(value, 0.0, 1.0);
    const uint8_t raw = static_cast<uint8_t>(std::lround(clamped * 255.0));
    return { raw };
}

inline double decode_probability(const std::vector<uint8_t>& bytes) {
    if (bytes.size() != 1) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    return static_cast<double>(bytes[0]) / 255.0;
}

inline std::vector<uint8_t> encode_percentage(double value) {
    double scaled = value;
    if (scaled <= 1.0) {
        scaled *= 100.0;
    }
    const double clamped = clamp(scaled, 0.0, 100.0);
    const uint8_t raw = static_cast<uint8_t>(std::lround(clamped));
    return { raw };
}

inline double decode_percentage(const std::vector<uint8_t>& bytes) {
    if (bytes.size() != 1) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    return static_cast<double>(bytes[0]) / 100.0;
}

inline std::vector<uint8_t> encode_imap(double value,
                                        double min,
                                        double max,
                                        size_t width) {
    if (width == 0 || width > 8) {
        return {};
    }
    const double lo = std::min(min, max);
    const double hi = std::max(min, max);
    const double span = hi - lo;
    if (span <= 0.0) {
        return std::vector<uint8_t>(width, 0u);
    }
    const double clamped = clamp(value, lo, hi);
    const uint64_t max_value = width == 8 ? std::numeric_limits<uint64_t>::max()
                                          : ((uint64_t{1} << (width * 8)) - 1);
    const double norm = (clamped - lo) / span;
    uint64_t raw = static_cast<uint64_t>(std::llround(norm * max_value));
    if (raw > max_value) {
        raw = max_value;
    }
    std::vector<uint8_t> bytes(width, 0u);
    for (size_t i = 0; i < width; ++i) {
        bytes[width - 1 - i] = static_cast<uint8_t>((raw >> (i * 8)) & 0xFFu);
    }
    return bytes;
}

inline double decode_imap(const std::vector<uint8_t>& bytes,
                          double min,
                          double max) {
    if (bytes.empty() || bytes.size() > 8) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    const double lo = std::min(min, max);
    const double hi = std::max(min, max);
    const double span = hi - lo;
    if (span <= 0.0) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    uint64_t raw = 0;
    for (uint8_t b : bytes) {
        raw = (raw << 8) | b;
    }
    const uint64_t max_value = bytes.size() == 8 ? std::numeric_limits<uint64_t>::max()
                                                 : ((uint64_t{1} << (bytes.size() * 8)) - 1);
    return lo + (static_cast<double>(raw) * (span / static_cast<double>(max_value)));
}

inline std::vector<uint8_t> encode_color(double value) {
    return encode_uint(value, 3);
}

inline double decode_color(const std::vector<uint8_t>& bytes) {
    return decode_uint(bytes, 3);
}

inline std::vector<uint8_t> encode_probability_percent(double value) {
    return encode_percentage(value);
}

inline double decode_probability_percent(const std::vector<uint8_t>& bytes) {
    return decode_percentage(bytes);
}

inline std::vector<uint8_t> encode_status(double value) {
    return encode_uint(value, 1);
}

inline double decode_status(const std::vector<uint8_t>& bytes) {
    return decode_uint(bytes, 1);
}

inline std::vector<uint8_t> encode_uint_width(double value, size_t width) {
    return encode_uint(value, width);
}

inline double decode_uint_width(const std::vector<uint8_t>& bytes, size_t width) {
    return decode_uint(bytes, width);
}

} // namespace detail

namespace {

std::vector<uint8_t> encode_ber_oid(uint64_t value) {
    std::vector<uint8_t> tmp;
    do {
        tmp.push_back(static_cast<uint8_t>(value & 0x7Fu));
        value >>= 7;
    } while (value > 0);

    std::vector<uint8_t> result;
    result.reserve(tmp.size());
    for (size_t i = tmp.size(); i-- > 0;) {
        uint8_t byte = tmp[i];
        if (i != 0) {
            byte |= 0x80u;
        }
        result.push_back(byte);
    }
    return result;
}

std::pair<uint64_t, size_t> decode_ber_oid(const std::vector<uint8_t>& bytes,
                                           size_t offset,
                                           size_t max_length) {
    if (offset >= bytes.size()) {
        throw std::runtime_error("BER OID offset out of range");
    }
    uint64_t value = 0;
    size_t consumed = 0;
    while (consumed < max_length) {
        const uint8_t byte = bytes[offset + consumed];
        value = (value << 7) | static_cast<uint64_t>(byte & 0x7Fu);
        ++consumed;
        if ((byte & 0x80u) == 0) {
            return { value, consumed };
        }
        if (consumed + offset >= bytes.size() && (byte & 0x80u)) {
            break;
        }
    }
    throw std::runtime_error("BER OID did not terminate before end of buffer");
}

} // namespace

void register_st0903(KLVRegistry& reg) {
    using namespace detail;

    reg.register_ul(VMTI_CHECKSUM, {
        [](double v) { return encode_uint_width(v, 2); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 2); }
    });

    reg.register_ul(VMTI_PRECISION_TIMESTAMP, {
        [](double v) { return encode_uint_width(v, 8); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 8); }
    });

    reg.register_ul(VMTI_LS_VERSION, {
        [](double v) { return encode_uint_width(v, 2); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 2); }
    });

    reg.register_ul(VMTI_TOTAL_TARGETS_DETECTED, {
        [](double v) { return encode_uint_width(v, 2); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 2); }
    });

    reg.register_ul(VMTI_NUM_TARGETS_REPORTED, {
        [](double v) { return encode_uint_width(v, 2); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 2); }
    });

    reg.register_ul(VMTI_FRAME_NUMBER, {
        [](double v) { return encode_uint_width(v, 4); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 4); }
    });

    reg.register_ul(VMTI_FRAME_WIDTH, {
        [](double v) { return encode_uint_width(v, 2); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 2); }
    });

    reg.register_ul(VMTI_FRAME_HEIGHT, {
        [](double v) { return encode_uint_width(v, 2); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 2); }
    });

    reg.register_ul(VMTI_HORIZONTAL_FOV, {
        [](double v) { return encode_imap(v, 0.0, 180.0, 2); },
        [](const std::vector<uint8_t>& bytes) { return decode_imap(bytes, 0.0, 180.0); }
    });

    reg.register_ul(VMTI_VERTICAL_FOV, {
        [](double v) { return encode_imap(v, 0.0, 180.0, 2); },
        [](const std::vector<uint8_t>& bytes) { return decode_imap(bytes, 0.0, 180.0); }
    });

    reg.register_ul(VTARGET_CENTROID, {
        [](double v) { return encode_uint_width(v, 4); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 4); }
    });

    reg.register_ul(VTARGET_BBOX_TOP_LEFT_PIXEL, {
        [](double v) { return encode_uint_width(v, 4); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 4); }
    });

    reg.register_ul(VTARGET_BBOX_BOTTOM_RIGHT_PIXEL, {
        [](double v) { return encode_uint_width(v, 4); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 4); }
    });

    reg.register_ul(VTARGET_PRIORITY, {
        [](double v) { return encode_uint_width(v, 1); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 1); }
    });

    reg.register_ul(VTARGET_CONFIDENCE_LEVEL, {
        [](double v) { return encode_probability_percent(v); },
        [](const std::vector<uint8_t>& bytes) { return decode_probability_percent(bytes); }
    });

    reg.register_ul(VTARGET_HISTORY, {
        [](double v) { return encode_uint_width(v, 2); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 2); }
    });

    reg.register_ul(VTARGET_PERCENT_TARGET_PIXELS, {
        [](double v) { return encode_probability_percent(v); },
        [](const std::vector<uint8_t>& bytes) { return decode_probability_percent(bytes); }
    });

    reg.register_ul(VTARGET_COLOR, {
        [](double v) { return encode_color(v); },
        [](const std::vector<uint8_t>& bytes) { return decode_color(bytes); }
    });

    reg.register_ul(VTARGET_INTENSITY, {
        [](double v) { return encode_uint_width(v, 3); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 3); }
    });

    reg.register_ul(VTARGET_LOCATION_OFFSET_LAT, {
        [](double v) { return encode_imap(v, -19.2, 19.2, 3); },
        [](const std::vector<uint8_t>& bytes) { return decode_imap(bytes, -19.2, 19.2); }
    });

    reg.register_ul(VTARGET_LOCATION_OFFSET_LON, {
        [](double v) { return encode_imap(v, -19.2, 19.2, 3); },
        [](const std::vector<uint8_t>& bytes) { return decode_imap(bytes, -19.2, 19.2); }
    });

    reg.register_ul(VTARGET_LOCATION_HAE, {
        [](double v) { return encode_imap(v, -900.0, 19000.0, 2); },
        [](const std::vector<uint8_t>& bytes) { return decode_imap(bytes, -900.0, 19000.0); }
    });

    reg.register_ul(VTARGET_BBOX_TOP_LEFT_LAT_OFFSET, {
        [](double v) { return encode_imap(v, -19.2, 19.2, 3); },
        [](const std::vector<uint8_t>& bytes) { return decode_imap(bytes, -19.2, 19.2); }
    });

    reg.register_ul(VTARGET_BBOX_TOP_LEFT_LON_OFFSET, {
        [](double v) { return encode_imap(v, -19.2, 19.2, 3); },
        [](const std::vector<uint8_t>& bytes) { return decode_imap(bytes, -19.2, 19.2); }
    });

    reg.register_ul(VTARGET_BBOX_BOTTOM_RIGHT_LAT_OFFSET, {
        [](double v) { return encode_imap(v, -19.2, 19.2, 3); },
        [](const std::vector<uint8_t>& bytes) { return decode_imap(bytes, -19.2, 19.2); }
    });

    reg.register_ul(VTARGET_BBOX_BOTTOM_RIGHT_LON_OFFSET, {
        [](double v) { return encode_imap(v, -19.2, 19.2, 3); },
        [](const std::vector<uint8_t>& bytes) { return decode_imap(bytes, -19.2, 19.2); }
    });

    reg.register_ul(VTARGET_CENTROID_ROW, {
        [](double v) { return encode_uint_width(v, 4); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 4); }
    });

    reg.register_ul(VTARGET_CENTROID_COLUMN, {
        [](double v) { return encode_uint_width(v, 4); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 4); }
    });

    reg.register_ul(VTARGET_ALGORITHM_ID, {
        [](double v) { return encode_uint_width(v, 3); },
        [](const std::vector<uint8_t>& bytes) { return decode_uint_width(bytes, 3); }
    });

    reg.register_ul(VTARGET_DETECTION_STATUS, {
        [](double v) { return encode_status(v); },
        [](const std::vector<uint8_t>& bytes) { return decode_status(bytes); }
    });
}

std::vector<uint8_t> encode_vtarget_series(const std::vector<VTargetPack>& packs) {
    std::vector<uint8_t> output;
    for (const auto& pack : packs) {
        auto pack_bytes = encode_ber_oid(pack.target_id);
        auto set_bytes = pack.set.encode();
        pack_bytes.insert(pack_bytes.end(), set_bytes.begin(), set_bytes.end());
        auto len_bytes = misb::encode_ber_length(pack_bytes.size());
        output.insert(output.end(), len_bytes.begin(), len_bytes.end());
        output.insert(output.end(), pack_bytes.begin(), pack_bytes.end());
    }
    return output;
}

std::vector<VTargetPack> decode_vtarget_series(const std::vector<uint8_t>& bytes) {
    std::vector<VTargetPack> packs;
    size_t offset = 0;
    while (offset < bytes.size()) {
        size_t pack_len = 0;
        size_t len_bytes = 0;
        if (!misb::decode_ber_length(bytes, offset, pack_len, len_bytes)) {
            throw std::runtime_error("Invalid BER length inside vTarget series");
        }
        offset += len_bytes;
        if (offset + pack_len > bytes.size()) {
            throw std::runtime_error("Truncated vTarget pack");
        }
        std::vector<uint8_t> pack_data(bytes.begin() + static_cast<long>(offset),
                                       bytes.begin() + static_cast<long>(offset + pack_len));
        auto oid_info = decode_ber_oid(pack_data, 0, pack_data.size());
        uint64_t target_id = oid_info.first;
        size_t oid_len = oid_info.second;
        if (oid_len > pack_data.size()) {
            throw std::runtime_error("Invalid OID length in vTarget pack");
        }
        std::vector<uint8_t> local_bytes(pack_data.begin() + static_cast<long>(oid_len),
                                         pack_data.end());
        KLVSet local(false, VTARGET_ST_ID);
        local.decode(local_bytes);
        VTargetPack pack;
        pack.target_id = target_id;
        pack.set = std::move(local);
        packs.push_back(std::move(pack));
        offset += pack_len;
    }
    return packs;
}

} // namespace st0903
} // namespace misb

