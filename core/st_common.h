#pragma once
#include <array>
#include <cstdint>
#include <vector>
#include <type_traits>
#include <cstddef>
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

// Encode a BER length field.
inline std::vector<uint8_t> encode_ber_length(size_t length) {
    if (length < 0x80) {
        return {static_cast<uint8_t>(length)};
    }
    std::vector<uint8_t> bytes;
    while (length > 0) {
        bytes.push_back(static_cast<uint8_t>(length & 0xFF));
        length >>= 8;
    }
    std::vector<uint8_t> out;
    out.push_back(static_cast<uint8_t>(0x80 | bytes.size()));
    for (auto it = bytes.rbegin(); it != bytes.rend(); ++it) {
        out.push_back(*it);
    }
    return out;
}

// Decode a BER length field starting at offset. Returns false on error.
inline bool decode_ber_length(const std::vector<uint8_t>& data,
                              size_t offset,
                              size_t& length,
                              size_t& len_bytes) {
    if (offset >= data.size()) return false;
    uint8_t first = data[offset];
    if ((first & 0x80) == 0) {
        length = first;
        len_bytes = 1;
        return true;
    }
    size_t count = first & 0x7F;
    if (offset + 1 + count > data.size()) return false;
    length = 0;
    for (size_t i = 0; i < count; ++i) {
        length = (length << 8) | data[offset + 1 + i];
    }
    len_bytes = 1 + count;
    return true;
}

} // namespace misb
