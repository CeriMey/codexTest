#include "stanag.h"
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

} // namespace stanag
