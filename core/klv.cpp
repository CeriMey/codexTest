#include "klv.h"
#include <algorithm>
#include <stdexcept>

KLVLeaf::KLVLeaf(const UL& ul, double value) : ul_(ul), value_(value) {}

std::vector<uint8_t> KLVLeaf::encode() const {
    auto* entry = KLVRegistry::instance().find(ul_);
    if (!entry) throw std::runtime_error("Unknown UL");
    auto data = entry->encoder(value_);
    std::vector<uint8_t> out;
    out.insert(out.end(), ul_.begin(), ul_.end());
    out.push_back(static_cast<uint8_t>(data.size()));
    out.insert(out.end(), data.begin(), data.end());
    return out;
}

void KLVLeaf::decode(const std::vector<uint8_t>& bytes) {
    if (bytes.size() < 17) throw std::runtime_error("Too short");
    UL ul;
    std::copy(bytes.begin(), bytes.begin() + 16, ul.begin());
    if (ul != ul_) throw std::runtime_error("UL mismatch");
    size_t len = bytes[16];
    if (bytes.size() < 17 + len) throw std::runtime_error("Length mismatch");
    std::vector<uint8_t> data(bytes.begin() + 17, bytes.begin() + 17 + len);
    auto* entry = KLVRegistry::instance().find(ul_);
    if (!entry) throw std::runtime_error("Unknown UL");
    value_ = entry->decoder(data);
}

KLVBytes::KLVBytes(const UL& ul, const std::vector<uint8_t>& value)
    : ul_(ul), value_(value) {}

std::vector<uint8_t> KLVBytes::encode() const {
    std::vector<uint8_t> out;
    out.insert(out.end(), ul_.begin(), ul_.end());
    out.push_back(static_cast<uint8_t>(value_.size()));
    out.insert(out.end(), value_.begin(), value_.end());
    return out;
}

void KLVBytes::decode(const std::vector<uint8_t>& bytes) {
    if (bytes.size() < 17) throw std::runtime_error("Too short");
    UL ul;
    std::copy(bytes.begin(), bytes.begin() + 16, ul.begin());
    if (ul != ul_) throw std::runtime_error("UL mismatch");
    size_t len = bytes[16];
    if (bytes.size() < 17 + len) throw std::runtime_error("Length mismatch");
    value_.assign(bytes.begin() + 17, bytes.begin() + 17 + len);
}

void KLVSet::add(std::shared_ptr<KLVNode> node) {
    children_.push_back(node);
}

std::vector<uint8_t> KLVSet::encode() const {
    std::vector<uint8_t> out;
    for (const auto& child : children_) {
        auto data = child->encode();
        out.insert(out.end(), data.begin(), data.end());
    }
    return out;
}

void KLVSet::decode(const std::vector<uint8_t>& data) {
    (void)data; // decoding composite not implemented
}

void KLVRegistry::register_ul(const UL& ul, const KLVEntry& entry) {
    entries_[ul] = entry;
}

const KLVEntry* KLVRegistry::find(const UL& ul) const {
    auto it = entries_.find(ul);
    if (it != entries_.end()) return &it->second;
    return nullptr;
}

KLVRegistry& KLVRegistry::instance() {
    static KLVRegistry inst;
    return inst;
}

