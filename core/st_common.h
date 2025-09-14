#pragma once
#include <array>
#include <cstdint>
#include <vector>
#include <type_traits>
#include "klv_types.h"

namespace misb {

// Build a synthetic UL for a given standard prefix and tag number.
constexpr UL make_st_ul(uint8_t standard, uint8_t tag) {
    return UL{0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,standard,0x00,0x00,tag};
}

// Big-endian pack for integral types
template <typename T,
          typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
inline std::vector<uint8_t> pack_be(T value) {
    std::vector<uint8_t> out(sizeof(T));
    using U = typename std::make_unsigned<T>::type;
    U v = static_cast<U>(value);
    for (size_t i = 0; i < sizeof(T); ++i) {
        out[sizeof(T)-1-i] = static_cast<uint8_t>((v >> (i*8)) & 0xFFu);
    }
    return out;
}

// Big-endian unpack. Returns false on size mismatch.
template <typename T,
          typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
inline bool unpack_be(const std::vector<uint8_t>& bytes, T& out) {
    if (bytes.size() != sizeof(T)) return false;
    using U = typename std::make_unsigned<T>::type;
    U v = 0;
    for (size_t i = 0; i < sizeof(T); ++i) {
        v = (v << 8) | static_cast<U>(bytes[i]);
    }
    out = static_cast<T>(v);
    return true;
}

} // namespace misb
