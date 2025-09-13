#pragma once

#include "klv.h"

namespace misb {
namespace st0601 {

// Universal Labels for a subset of MISB ST0601 tags
extern const UL PLATFORM_HEADING_ANGLE;
extern const UL TARGET_LOCATION_ELEVATION;
extern const UL TARGET_TRACK_GATE_WIDTH;
extern const UL TARGET_TRACK_GATE_HEIGHT;

// Register encode/decode lambdas for the above ULs
void register_st0601(KLVRegistry& reg);

} // namespace st0601
} // namespace misb
