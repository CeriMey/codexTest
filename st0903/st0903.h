#pragma once

#include "klv.h"

namespace misb {
namespace st0903 {

// Universal Labels for a subset of MISB ST0903 tags
extern const UL VMTI_TARGET_ID;
extern const UL VMTI_DETECTION_STATUS;
extern const UL VMTI_DETECTION_PROBABILITY;
extern const UL VMTI_TRACKER_ID;
extern const UL VMTI_CLASS_ID;
extern const UL VMTI_TOTAL_TARGETS_DETECTED;
extern const UL VMTI_NUM_TARGETS_REPORTED;
extern const UL VMTI_FRAME_WIDTH;
extern const UL VMTI_FRAME_HEIGHT;
extern const UL VMTI_SOURCE_SENSOR;
extern const UL VMTI_HORIZONTAL_FOV;
extern const UL VMTI_VERTICAL_FOV;
extern const UL VMTI_MIIS_ID;
extern const UL VMTI_CENTROID_ROW;
extern const UL VMTI_CENTROID_COL;
extern const UL VMTI_ALGORITHM_ID;

// Register encode/decode lambdas for the above ULs
void register_st0903(KLVRegistry& reg);

} // namespace st0903
} // namespace misb

