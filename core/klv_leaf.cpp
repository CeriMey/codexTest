#include "klv_leaf.h"
#include "klv_registry.h"
#include "st_common.h"
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
        auto len_bytes = misb::encode_ber_length(data.size());
        out.insert(out.end(), len_bytes.begin(), len_bytes.end());
    } else {
        out.insert(out.end(), ul_.begin(), ul_.end());
        auto len_bytes = misb::encode_ber_length(data.size());
        out.insert(out.end(), len_bytes.begin(), len_bytes.end());
    }
    out.insert(out.end(), data.begin(), data.end());
    return out;
}

void KLVLeaf::decode(const std::vector<uint8_t>& bytes) {
    if (use_tag_) {
        if (bytes.size() < 2) throw std::runtime_error("Too short");
        if (bytes[0] != ul_[15]) throw std::runtime_error("Tag mismatch");
        size_t len = 0, len_bytes = 0;
        if (!misb::decode_ber_length(bytes, 1, len, len_bytes))
            throw std::runtime_error("Length parse error");
        if (bytes.size() < 1 + len_bytes + len)
            throw std::runtime_error("Length mismatch");
        std::vector<uint8_t> data(bytes.begin() + 1 + len_bytes,
                                  bytes.begin() + 1 + len_bytes + len);
        auto* entry = KLVRegistry::instance().find(ul_);
        if (!entry) throw std::runtime_error("Unknown UL");
        value_ = entry->decoder(data);
    } else {
        if (bytes.size() < 18) throw std::runtime_error("Too short");
        UL ul;
        std::copy(bytes.begin(), bytes.begin() + 16, ul.begin());
        if (ul != ul_) throw std::runtime_error("UL mismatch");
        size_t len = 0, len_bytes = 0;
        if (!misb::decode_ber_length(bytes, 16, len, len_bytes))
            throw std::runtime_error("Length parse error");
        if (bytes.size() < 16 + len_bytes + len)
            throw std::runtime_error("Length mismatch");
        std::vector<uint8_t> data(bytes.begin() + 16 + len_bytes,
                                  bytes.begin() + 16 + len_bytes + len);
        auto* entry = KLVRegistry::instance().find(ul_);
        if (!entry) throw std::runtime_error("Unknown UL");
        value_ = entry->decoder(data);
    }
}
