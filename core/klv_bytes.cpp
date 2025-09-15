#include "klv_bytes.h"
#include <algorithm>
#include <stdexcept>

KLVBytes::KLVBytes(const UL& ul, const std::vector<uint8_t>& value, bool use_tag)
    : ul_(ul), value_(value), use_tag_(use_tag) {}

std::vector<uint8_t> KLVBytes::encode() const {
    std::vector<uint8_t> out;
    if (use_tag_) {
        out.push_back(ul_[15]);
        out.push_back(static_cast<uint8_t>(value_.size() & 0xFF));
    } else {
        out.insert(out.end(), ul_.begin(), ul_.end());
        out.push_back(static_cast<uint8_t>((value_.size() >> 8) & 0xFF));
        out.push_back(static_cast<uint8_t>(value_.size() & 0xFF));
    }
    out.insert(out.end(), value_.begin(), value_.end());
    return out;
}

void KLVBytes::decode(const std::vector<uint8_t>& bytes) {
    if (use_tag_) {
        if (bytes.size() < 2) throw std::runtime_error("Too short");
        if (bytes[0] != ul_[15]) throw std::runtime_error("Tag mismatch");
        size_t len = bytes[1];
        if (bytes.size() < 2 + len) throw std::runtime_error("Length mismatch");
        value_.assign(bytes.begin() + 2, bytes.begin() + 2 + len);
    } else {
        if (bytes.size() < 18) throw std::runtime_error("Too short");
        UL ul;
        std::copy(bytes.begin(), bytes.begin() + 16, ul.begin());
        if (ul != ul_) throw std::runtime_error("UL mismatch");
        size_t len = (static_cast<size_t>(bytes[16]) << 8) | bytes[17];
        if (bytes.size() < 18 + len) throw std::runtime_error("Length mismatch");
        value_.assign(bytes.begin() + 18, bytes.begin() + 18 + len);
    }
}
