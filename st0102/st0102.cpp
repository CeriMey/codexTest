#include "st0102.h"

namespace misb {
namespace st0102 {

// Dummy UL values for demonstration purposes
const UL CLASSIFICATION        = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x00,0x00,0x00};
const UL CLASSIFICATION_SYSTEM = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x00,0x00,0x01};

void register_st0102(KLVRegistry& reg) {
    // Classification: enumeration -> uint8
    reg.register_ul(CLASSIFICATION, {
        [](double code) {
            uint8_t raw = static_cast<uint8_t>(code);
            return std::vector<uint8_t>{raw};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return static_cast<double>(bytes[0]);
        }
    });

    // Classification System: enumeration -> uint8
    reg.register_ul(CLASSIFICATION_SYSTEM, {
        [](double code) {
            uint8_t raw = static_cast<uint8_t>(code);
            return std::vector<uint8_t>{raw};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return static_cast<double>(bytes[0]);
        }
    });
}

} // namespace st0102
} // namespace misb

