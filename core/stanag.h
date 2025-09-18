#pragma once

#include "klv.h"
#include <cstring>
#include <memory>
#include <string>
#include <vector>

namespace stanag {

// UL identifying the UAS Datalink Local Set as defined by STANAG 4609
constexpr UL UAS_DATALINK_LOCAL_SET_UL = {
    0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,
    0x0E,0x01,0x03,0x01,0x01,0x00,0x00,0x00
};

struct TagValue {
    enum class Kind {
        Numeric,
        Dataset,
        Bytes
    };

    UL ul;
    Kind kind;
    double value;
    std::shared_ptr<KLVSet> set;
    std::vector<uint8_t> bytes;

    TagValue(const UL& u, double v)
        : ul(u), kind(Kind::Numeric), value(v), set(nullptr), bytes() {}

    TagValue(const UL& u, const KLVSet& s)
        : ul(u), kind(Kind::Dataset), value(0.0), set(std::make_shared<KLVSet>(s)), bytes() {}

    TagValue(const UL& u, const std::vector<uint8_t>& b)
        : ul(u), kind(Kind::Bytes), value(0.0), set(nullptr), bytes(b) {}

    TagValue(const UL& u, const std::string& text)
        : TagValue(u, std::vector<uint8_t>(text.begin(), text.end())) {}

    TagValue(const UL& u, const char* text)
        : TagValue(u, to_bytes(text)) {}

private:
    static std::vector<uint8_t> to_bytes(const char* text) {
        std::vector<uint8_t> out;
        if (!text) {
            return out;
        }
        size_t len = std::strlen(text);
        out.reserve(len);
        for (size_t i = 0; i < len; ++i) {
            out.push_back(static_cast<uint8_t>(static_cast<unsigned char>(text[i])));
        }
        return out;
    }
};

KLVSet create_dataset(const std::vector<TagValue>& tags, bool use_ul = true);

// Assemble a complete STANAG 4609 packet with the outer UAS Datalink UL
std::vector<uint8_t> create_stanag4609_packet(const std::vector<TagValue>& tags);

} // namespace stanag
