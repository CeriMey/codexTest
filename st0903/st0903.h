#pragma once

#include "klv.h"
#include "st_common.h"

namespace misb {
namespace st0903 {

constexpr uint8_t ST_ID = 0x03;
constexpr UL VMTI_TARGET_ID        = make_st_ul(ST_ID, 0x00);
constexpr UL VMTI_DETECTION_STATUS = make_st_ul(ST_ID, 0x01);
constexpr UL VMTI_DETECTION_PROBABILITY = make_st_ul(ST_ID, 0x02);
constexpr UL VMTI_TRACKER_ID       = make_st_ul(ST_ID, 0x03);
constexpr UL VMTI_CLASS_ID         = make_st_ul(ST_ID, 0x04);
constexpr UL VMTI_TOTAL_TARGETS_DETECTED = make_st_ul(ST_ID, 0x05);
constexpr UL VMTI_NUM_TARGETS_REPORTED  = make_st_ul(ST_ID, 0x06);
constexpr UL VMTI_FRAME_WIDTH      = make_st_ul(ST_ID, 0x07);
constexpr UL VMTI_FRAME_HEIGHT     = make_st_ul(ST_ID, 0x08);
constexpr UL VMTI_SOURCE_SENSOR    = make_st_ul(ST_ID, 0x09);
constexpr UL VMTI_HORIZONTAL_FOV   = make_st_ul(ST_ID, 0x0A);
constexpr UL VMTI_VERTICAL_FOV     = make_st_ul(ST_ID, 0x0B);
constexpr UL VMTI_MIIS_ID          = make_st_ul(ST_ID, 0x0C);
constexpr UL VMTI_CENTROID_ROW     = make_st_ul(ST_ID, 0x0D);
constexpr UL VMTI_CENTROID_COL     = make_st_ul(ST_ID, 0x0E);
constexpr UL VMTI_ALGORITHM_ID     = make_st_ul(ST_ID, 0x0F);

// Register encode/decode lambdas for the above ULs
void register_st0903(KLVRegistry& reg);

} // namespace st0903
} // namespace misb

