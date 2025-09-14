#include "st0102.h"

namespace misb {
namespace st0102 {

void register_st0102(KLVRegistry& reg) {
    // Classification: enumeration -> uint8
    reg.register_ul(CLASSIFICATION, {
        [](double code) {
            return pack_be<uint8_t>(static_cast<uint8_t>(code));
        },
        [](const std::vector<uint8_t>& bytes) {
            uint8_t raw{};
            if (!unpack_be<uint8_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });

    // Classification System: enumeration -> uint8
    reg.register_ul(CLASSIFICATION_SYSTEM, {
        [](double code) {
            return pack_be<uint8_t>(static_cast<uint8_t>(code));
        },
        [](const std::vector<uint8_t>& bytes) {
            uint8_t raw{};
            if (!unpack_be<uint8_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });
}

} // namespace st0102
} // namespace misb

