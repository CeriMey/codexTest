#include "klv_set.h"
#include "klv_leaf.h"
#include "klv_bytes.h"
#include "klv_registry.h"
#include "st_common.h"
#include <algorithm>

KLVSet::KLVSet(bool use_ul_keys, uint8_t st_id)
    : use_ul_keys_(use_ul_keys), st_id_(st_id) {}

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
    while (true) {
        if (use_ul_keys_) {
            if (i + 18 > data.size()) break;
            UL ul;
            std::copy(data.begin() + i, data.begin() + i + 16, ul.begin());
            i += 16;
            size_t len = 0, len_bytes = 0;
            if (!misb::decode_ber_length(data, i, len, len_bytes)) break;
            i += len_bytes;
            if (i + len > data.size()) break;
            std::vector<uint8_t> value(data.begin() + i, data.begin() + i + len);
            i += len;

            const KLVEntry* entry = KLVRegistry::instance().find(ul);
            if (entry) {
                std::vector<uint8_t> item;
                item.insert(item.end(), ul.begin(), ul.end());
                auto len_vec = misb::encode_ber_length(len);
                item.insert(item.end(), len_vec.begin(), len_vec.end());
                item.insert(item.end(), value.begin(), value.end());
                auto leaf = std::make_shared<KLVLeaf>(ul);
                leaf->decode(item);
                children_.push_back(leaf);
            } else {
                auto bytes = std::make_shared<KLVBytes>(ul, value);
                children_.push_back(bytes);
            }
        } else {
            if (i + 2 > data.size()) break;
            uint8_t tag = data[i++];
            size_t len = data[i++];
            if (i + len > data.size()) break;
            UL ul = misb::make_st_ul(st_id_, tag);
            std::vector<uint8_t> value(data.begin() + i, data.begin() + i + len);
            i += len;

            const KLVEntry* entry = KLVRegistry::instance().find(ul);
            if (entry) {
                std::vector<uint8_t> item;
                item.push_back(tag);
                item.push_back(static_cast<uint8_t>(len));
                item.insert(item.end(), value.begin(), value.end());
                auto leaf = std::make_shared<KLVLeaf>(ul, 0.0, true);
                leaf->decode(item);
                children_.push_back(leaf);
            } else {
                auto bytes = std::make_shared<KLVBytes>(ul, value, true);
                children_.push_back(bytes);
            }
        }
    }
}
