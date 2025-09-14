#pragma once
#include "klv_types.h"
#include <map>
#include <vector>
#include <functional>

struct KLVEntry {
    std::function<std::vector<uint8_t>(double)> encoder;
    std::function<double(const std::vector<uint8_t>&)> decoder;
};

class KLVRegistry {
public:
    void register_ul(const UL& ul, const KLVEntry& entry);
    const KLVEntry* find(const UL& ul) const;
    static KLVRegistry& instance();
private:
    std::map<UL, KLVEntry> entries_;
};
