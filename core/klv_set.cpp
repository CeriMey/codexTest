#include "klv_set.h"
#include "klv_leaf.h"
#include "klv_bytes.h"
#include "klv_registry.h"
#include <algorithm>

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
    children_.clear();
    size_t i = 0;
    while (i + 18 <= data.size()) {
        UL ul;
        std::copy(data.begin() + i, data.begin() + i + 16, ul.begin());
        i += 16;
        if (i + 2 > data.size()) break;
        size_t len = (static_cast<size_t>(data[i]) << 8) | data[i + 1];
        i += 2;
        if (i + len > data.size()) break;
        std::vector<uint8_t> value(data.begin() + i, data.begin() + i + len);
        i += len;

        const KLVEntry* entry = KLVRegistry::instance().find(ul);
        if (entry) {
            std::vector<uint8_t> item;
            item.insert(item.end(), ul.begin(), ul.end());
            item.push_back(static_cast<uint8_t>((len >> 8) & 0xFF));
            item.push_back(static_cast<uint8_t>(len & 0xFF));
            item.insert(item.end(), value.begin(), value.end());
            auto leaf = std::make_shared<KLVLeaf>(ul);
            leaf->decode(item);
            children_.push_back(leaf);
        } else {
            auto bytes = std::make_shared<KLVBytes>(ul, value);
            children_.push_back(bytes);
        }
    }
}
