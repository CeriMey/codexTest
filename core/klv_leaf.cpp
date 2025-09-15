#include "klv_leaf.h"
#include "klv_registry.h"
#include <algorithm>
#include <stdexcept>

KLVLeaf::KLVLeaf(const UL& ul, double value, bool use_tag)
    : ul_(ul), value_(value), use_tag_(use_tag) {}

std::vector<uint8_t> KLVLeaf::encode() const {
    auto* entry = KLVRegistry::instance().find(ul_);
    if (!entry) throw std::runtime_error("Unknown UL");
    auto data = entry->encoder(value_);
    std::vector<uint8_t> out;
    if (use_tag_) {
        out.push_back(ul_[15]);
        out.push_back(static_cast<uint8_t>(data.size() & 0xFF));
    } else {
        out.insert(out.end(), ul_.begin(), ul_.end());
        out.push_back(static_cast<uint8_t>((data.size() >> 8) & 0xFF));
        out.push_back(static_cast<uint8_t>(data.size() & 0xFF));
    }
    out.insert(out.end(), data.begin(), data.end());
    return out;
}

void KLVLeaf::decode(const std::vector<uint8_t>& bytes) {
    if (use_tag_) {
        if (bytes.size() < 2) throw std::runtime_error("Too short");
        if (bytes[0] != ul_[15]) throw std::runtime_error("Tag mismatch");
        size_t len = bytes[1];
        if (bytes.size() < 2 + len) throw std::runtime_error("Length mismatch");
        std::vector<uint8_t> data(bytes.begin() + 2, bytes.begin() + 2 + len);
        auto* entry = KLVRegistry::instance().find(ul_);
        if (!entry) throw std::runtime_error("Unknown UL");
        value_ = entry->decoder(data);
    } else {
        if (bytes.size() < 18) throw std::runtime_error("Too short");
        UL ul;
        std::copy(bytes.begin(), bytes.begin() + 16, ul.begin());
        if (ul != ul_) throw std::runtime_error("UL mismatch");
        size_t len = (static_cast<size_t>(bytes[16]) << 8) | bytes[17];
        if (bytes.size() < 18 + len) throw std::runtime_error("Length mismatch");
        std::vector<uint8_t> data(bytes.begin() + 18, bytes.begin() + 18 + len);
        auto* entry = KLVRegistry::instance().find(ul_);
        if (!entry) throw std::runtime_error("Unknown UL");
        value_ = entry->decoder(data);
    }
}
