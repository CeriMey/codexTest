#include "klv_registry.h"

void KLVRegistry::register_ul(const UL& ul, const KLVEntry& entry) {
    entries_[ul] = entry;
}

const KLVEntry* KLVRegistry::find(const UL& ul) const {
    auto it = entries_.find(ul);
    if (it != entries_.end()) return &it->second;
    return nullptr;
}

KLVRegistry& KLVRegistry::instance() {
    static KLVRegistry inst;
    return inst;
}
