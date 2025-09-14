#include "klv_bytes.h"
#include <algorithm>
#include <stdexcept>

KLVBytes::KLVBytes(const UL& ul, const std::vector<uint8_t>& value)
    : ul_(ul), value_(value) {}

std::vector<uint8_t> KLVBytes::encode() const {
    std::vector<uint8_t> out;
    out.insert(out.end(), ul_.begin(), ul_.end());
    out.push_back(static_cast<uint8_t>((value_.size() >> 8) & 0xFF));
    out.push_back(static_cast<uint8_t>(value_.size() & 0xFF));
    out.insert(out.end(), value_.begin(), value_.end());
    return out;
}

void KLVBytes::decode(const std::vector<uint8_t>& bytes) {
    if (bytes.size() < 18) throw std::runtime_error("Too short");
    UL ul;
    std::copy(bytes.begin(), bytes.begin() + 16, ul.begin());
    if (ul != ul_) throw std::runtime_error("UL mismatch");
    size_t len = (static_cast<size_t>(bytes[16]) << 8) | bytes[17];
    if (bytes.size() < 18 + len) throw std::runtime_error("Length mismatch");
    value_.assign(bytes.begin() + 18, bytes.begin() + 18 + len);
}
