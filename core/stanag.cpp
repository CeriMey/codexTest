#include "stanag.h"
#include "st_common.h"
#include "st0601.h"
#include <algorithm>
#include <memory>

namespace stanag {

KLVSet create_dataset(const std::vector<TagValue>& tags, bool use_ul) {
    uint8_t st_id = 0;
    if (!tags.empty()) st_id = tags[0].ul[12];
    KLVSet set(use_ul, st_id);
    for (const auto& t : tags) {
        switch (t.kind) {
        case TagValue::Kind::Numeric:
            set.add(std::make_shared<KLVLeaf>(t.ul, t.value, !use_ul));
            break;
        case TagValue::Kind::Dataset:
            if (t.set) {
                set.add(std::make_shared<KLVBytes>(t.ul, t.set->encode(), !use_ul));
            }
            break;
        case TagValue::Kind::Bytes:
            set.add(std::make_shared<KLVBytes>(t.ul, t.bytes, !use_ul));
            break;
        }
    }
    return set;
}

std::vector<uint8_t> create_stanag4609_packet(const std::vector<TagValue>& tags) {
    // Build payload without checksum
    KLVSet payload_set = create_dataset(tags, false);
    auto payload = payload_set.encode();

    std::vector<uint8_t> out;
    out.insert(out.end(), UAS_DATALINK_LOCAL_SET_UL.begin(),
               UAS_DATALINK_LOCAL_SET_UL.end());

    // Account for trailing checksum TLV (tag 1)
    size_t payload_with_crc = payload.size() + 4;
    auto len_bytes = misb::encode_ber_length(payload_with_crc);
    out.insert(out.end(), len_bytes.begin(), len_bytes.end());
    out.insert(out.end(), payload.begin(), payload.end());

    // Append checksum tag and length
    out.push_back(0x01);
    out.push_back(0x02);

    // Compute checksum over everything so far
    uint16_t crc = misb::klv_checksum_16(out);
    out.push_back(static_cast<uint8_t>((crc >> 8) & 0xFF));
    out.push_back(static_cast<uint8_t>(crc & 0xFF));

    return out;
}

CompositeBuilder& CompositeBuilder::add_numeric(const UL& ul, double value) {
    tags_.emplace_back(ul, value);
    return *this;
}

CompositeBuilder& CompositeBuilder::add_bytes(const UL& ul,
                                               const std::vector<uint8_t>& bytes) {
    tags_.emplace_back(ul, bytes);
    return *this;
}

CompositeBuilder& CompositeBuilder::add_string(const UL& ul, const std::string& text) {
    tags_.emplace_back(ul, text);
    return *this;
}

CompositeBuilder& CompositeBuilder::add_cstring(const UL& ul, const char* text) {
    tags_.emplace_back(ul, text);
    return *this;
}

CompositeBuilder& CompositeBuilder::add_dataset(const UL& ul,
                                                 const CompositeBuilder& builder,
                                                 bool use_ul) {
    tags_.emplace_back(ul, builder.as_dataset(use_ul));
    return *this;
}

CompositeBuilder& CompositeBuilder::add_dataset(const UL& ul, const KLVSet& set) {
    tags_.emplace_back(ul, set);
    return *this;
}

CompositeBuilder& CompositeBuilder::add_tag(const TagValue& tag) {
    tags_.push_back(tag);
    return *this;
}

KLVSet CompositeBuilder::as_dataset(bool use_ul) const {
    return create_dataset(tags_, use_ul);
}

std::vector<uint8_t> CompositeBuilder::as_packet(bool ensure_version) const {
    std::vector<TagValue> items = tags_;
    if (ensure_version) {
        const UL version_ul = misb::st0601::UAS_LS_VERSION_NUMBER;
        bool has_version = std::any_of(items.begin(), items.end(),
                                       [&](const TagValue& tag) {
                                           return tag.ul == version_ul;
                                       });
        if (!has_version) {
            items.emplace_back(version_ul, 12.0);
        }
    }
    return create_stanag4609_packet(items);
}

} // namespace stanag
