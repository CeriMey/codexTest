#include "st0903.h"

namespace misb {
namespace st0903 {

// Dummy UL values for demonstration purposes
const UL VMTI_TARGET_ID        = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x00};
const UL VMTI_DETECTION_STATUS = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x01};
const UL VMTI_DETECTION_PROBABILITY = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x02};

void register_st0903(KLVRegistry& reg) {
    // VMTI Target ID: uint16 big-endian
    reg.register_ul(VMTI_TARGET_ID, {
        [](double id) {
            uint16_t raw = static_cast<uint16_t>(id);
            return std::vector<uint8_t>{
                static_cast<uint8_t>((raw >> 8) & 0xFF),
                static_cast<uint8_t>(raw & 0xFF)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = (static_cast<uint16_t>(bytes[0]) << 8) | bytes[1];
            return static_cast<double>(raw);
        }
    });

    // VMTI Detection Status: enumeration -> uint8
    reg.register_ul(VMTI_DETECTION_STATUS, {
        [](double status) {
            uint8_t raw = static_cast<uint8_t>(status);
            return std::vector<uint8_t>{raw};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return static_cast<double>(bytes[0]);
        }
    });

    // VMTI Detection Probability: [0,1] -> uint8
    reg.register_ul(VMTI_DETECTION_PROBABILITY, {
        [](double prob) {
            if (prob < 0.0) prob = 0.0;
            if (prob > 1.0) prob = 1.0;
            uint8_t raw = static_cast<uint8_t>(prob * 255.0);
            return std::vector<uint8_t>{raw};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return static_cast<double>(bytes[0]) / 255.0;
        }
    });
}

} // namespace st0903
} // namespace misb

