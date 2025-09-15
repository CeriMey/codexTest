#pragma once

#include "klv.h"
#include <vector>
#include <memory>

namespace stanag {

// UL identifying the UAS Datalink Local Set as defined by STANAG 4609
constexpr UL UAS_DATALINK_LOCAL_SET_UL = {
    0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,
    0x0E,0x01,0x03,0x01,0x01,0x00,0x00,0x00
};

struct TagValue {
    UL ul;
    double value;
    std::shared_ptr<KLVSet> set;

    TagValue(const UL& u, double v)
        : ul(u), value(v), set(nullptr) {}

    TagValue(const UL& u, const KLVSet& s)
        : ul(u), value(0.0), set(std::make_shared<KLVSet>(s)) {}
};

KLVSet create_dataset(const std::vector<TagValue>& tags, bool use_ul = true, bool with_crc = false);

// Assemble a complete STANAG 4609 packet with the outer UAS Datalink UL
std::vector<uint8_t> create_stanag4609_packet(const std::vector<TagValue>& tags);

} // namespace stanag
