#include "st0601.h"

namespace misb {
namespace st0601 {

// Dummy UL values for demonstration purposes
const UL PLATFORM_HEADING_ANGLE   = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00};
const UL TARGET_LOCATION_ELEVATION = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01};
const UL TARGET_TRACK_GATE_WIDTH   = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x02};
const UL TARGET_TRACK_GATE_HEIGHT  = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x03};

void register_st0601(KLVRegistry& reg) {
    // Platform Heading Angle: degrees [0,360] -> uint16
    reg.register_ul(PLATFORM_HEADING_ANGLE, {
        [](double degrees) {
            uint16_t raw = static_cast<uint16_t>((degrees / 360.0) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return (raw / 65535.0) * 360.0;
        }
    });

    // Target Location Elevation: meters [-900,19000] -> uint16
    reg.register_ul(TARGET_LOCATION_ELEVATION, {
        [](double meters) {
            double min = -900.0, max = 19000.0;
            uint16_t raw = static_cast<uint16_t>(((meters - min) / (max - min)) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            double min = -900.0, max = 19000.0;
            return min + (raw / 65535.0) * (max - min);
        }
    });

    // Target Track Gate Width: pixels [0,512] -> uint8
    reg.register_ul(TARGET_TRACK_GATE_WIDTH, {
        [](double pixels) {
            uint8_t raw = static_cast<uint8_t>((pixels / 512.0) * 255.0);
            return std::vector<uint8_t>{raw};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return (bytes[0] / 255.0) * 512.0;
        }
    });

    // Target Track Gate Height: pixels [0,512] -> uint8
    reg.register_ul(TARGET_TRACK_GATE_HEIGHT, {
        [](double pixels) {
            uint8_t raw = static_cast<uint8_t>((pixels / 512.0) * 255.0);
            return std::vector<uint8_t>{raw};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return (bytes[0] / 255.0) * 512.0;
        }
    });
}

} // namespace st0601
} // namespace misb
