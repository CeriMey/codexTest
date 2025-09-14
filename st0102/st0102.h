#pragma once

#include "klv.h"

namespace misb {
namespace st0102 {

// Universal Labels for a subset of MISB ST0102 tags
extern const UL CLASSIFICATION;
extern const UL CLASSIFICATION_SYSTEM;

// Register encode/decode lambdas for the above ULs
void register_st0102(KLVRegistry& reg);

} // namespace st0102
} // namespace misb

