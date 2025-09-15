#include "klv_set.h"
#include "klv_leaf.h"
#include "klv_bytes.h"
#include "klv_registry.h"
#include "st_common.h"
#include <algorithm>
#include <stdexcept>

namespace {
uint16_t crc16_ccitt(const std::vector<uint8_t>& data) {
    uint16_t crc = 0xFFFF;
    for (uint8_t b : data) {
        crc ^= static_cast<uint16_t>(b) << 8;
        for (int i = 0; i < 8; ++i) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}
}

KLVSet::KLVSet(bool use_ul_keys, uint8_t st_id, bool with_crc)
    : use_ul_keys_(use_ul_keys), st_id_(st_id), with_crc_(with_crc) {}

void KLVSet::add(std::shared_ptr<KLVNode> node) {
    children_.push_back(node);
}

std::vector<uint8_t> KLVSet::encode() const {
    std::vector<uint8_t> out;
    for (const auto& child : children_) {
        auto data = child->encode();
        out.insert(out.end(), data.begin(), data.end());
    }
    if (!use_ul_keys_ && with_crc_) {
        uint16_t crc = crc16_ccitt(out);
        out.push_back(0x01); // CRC tag
        out.push_back(0x02); // length
        out.push_back(static_cast<uint8_t>((crc >> 8) & 0xFF));
        out.push_back(static_cast<uint8_t>(crc & 0xFF));
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
        } else {
            if (i + 2 > data.size()) break;
            uint8_t tag = data[i++];
            if (with_crc_ && tag == 0x01 && i + 3 == data.size()) {
                size_t len = data[i++];
                if (len != 2 || i + 2 > data.size()) break;
                uint16_t crc_val = (static_cast<uint16_t>(data[i]) << 8) | data[i + 1];
                uint16_t crc_calc = crc16_ccitt(std::vector<uint8_t>(data.begin(), data.begin() + i - 2));
                if (crc_val != crc_calc) throw std::runtime_error("CRC mismatch");
                break;
            }
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
