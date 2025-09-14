#include "st0601.h"

namespace misb {
namespace st0601 {

void register_st0601(KLVRegistry& reg) {
    // Unix Timestamp: microseconds since epoch -> uint64
    reg.register_ul(UNIX_TIMESTAMP, {
        [](double micros) {
            return pack_be<uint64_t>(static_cast<uint64_t>(micros));
        },
        [](const std::vector<uint8_t>& bytes) {
            uint64_t raw{};
            if (!unpack_be<uint64_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });

    // Platform Heading Angle: degrees [0,360] -> uint16
    reg.register_ul(PLATFORM_HEADING_ANGLE, {
        [](double degrees) {
            uint16_t raw = static_cast<uint16_t>((degrees / 360.0) * 65535.0);
            return pack_be<uint16_t>(raw);
        },
        [](const std::vector<uint8_t>& bytes) {
            uint16_t raw{};
            if (!unpack_be<uint16_t>(bytes, raw)) return 0.0;
            return (static_cast<double>(raw) / 65535.0) * 360.0;
        }
    });
}

} // namespace st0601
} // namespace misb

