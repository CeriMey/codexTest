#pragma once

#include "klv.h"
#include <vector>

namespace stanag {

struct TagValue {
    UL ul;
    double value;
};

KLVSet create_dataset(const std::vector<TagValue>& tags);

} // namespace stanag
