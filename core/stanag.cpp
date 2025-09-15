#include "stanag.h"
#include "st_common.h"
#include <memory>

namespace stanag {

KLVSet create_dataset(const std::vector<TagValue>& tags, bool use_ul) {
    uint8_t st_id = 0;
    if (!tags.empty()) st_id = tags[0].ul[12];
    KLVSet set(use_ul, st_id);
    for (const auto& t : tags) {
        if (t.set) {
            set.add(std::make_shared<KLVBytes>(t.ul, t.set->encode(), !use_ul));
        } else {
            set.add(std::make_shared<KLVLeaf>(t.ul, t.value, !use_ul));
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

} // namespace stanag
