#pragma once

#include "klv.h"
#include <vector>
#include <memory>

namespace stanag {

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

} // namespace stanag
