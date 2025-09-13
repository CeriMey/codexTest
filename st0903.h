#pragma once

#include "klv.h"

namespace misb {
namespace st0903 {

// Universal Labels for a subset of MISB ST0903 tags
extern const UL VMTI_TARGET_ID;
extern const UL VMTI_DETECTION_STATUS;

// Register encode/decode lambdas for the above ULs
void register_st0903(KLVRegistry& reg);

} // namespace st0903
} // namespace misb

