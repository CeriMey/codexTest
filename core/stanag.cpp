#include "stanag.h"
#include <memory>

namespace stanag {

KLVSet create_dataset(const std::vector<TagValue>& tags) {
    KLVSet set;
    for (const auto& t : tags) {
        if (t.set) {
            set.add(std::make_shared<KLVBytes>(t.ul, t.set->encode()));
        } else {
            set.add(std::make_shared<KLVLeaf>(t.ul, t.value));
        }
    }
    return set;
}

} // namespace stanag
