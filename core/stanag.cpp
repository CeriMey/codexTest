#include "stanag.h"
#include "st_common.h"
#include <memory>

namespace stanag {

KLVSet create_dataset(const std::vector<TagValue>& tags, bool use_ul, bool with_crc) {
    uint8_t st_id = 0;
    if (!tags.empty()) st_id = tags[0].ul[12];
    KLVSet set(use_ul, st_id, with_crc);
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
    // ST0601 payload uses local tags with a trailing CRC
    KLVSet payload_set = create_dataset(tags, false, true);
    auto payload = payload_set.encode();
    std::vector<uint8_t> out;
    out.insert(out.end(), UAS_DATALINK_LOCAL_SET_UL.begin(),
               UAS_DATALINK_LOCAL_SET_UL.end());
    auto len_bytes = misb::encode_ber_length(payload.size());
    out.insert(out.end(), len_bytes.begin(), len_bytes.end());
    out.insert(out.end(), payload.begin(), payload.end());
    return out;
}

} // namespace stanag
