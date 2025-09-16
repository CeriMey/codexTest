#pragma once

#include "klv.h"
#include "st_common.h"

#include <cstdint>
#include <initializer_list>
#include <vector>

namespace misb {
namespace st0903 {

constexpr uint8_t ST_ID = 0x03;
constexpr uint8_t VTARGET_ST_ID = 0x13;
constexpr uint8_t ALGORITHM_ST_ID = 0x14;
constexpr uint8_t ONTOLOGY_ST_ID = 0x15;

#define ST0903_LOCAL_SET_TAGS(X) \
    X(VMTI_CHECKSUM, 1) \
    X(VMTI_PRECISION_TIMESTAMP, 2) \
    X(VMTI_SYSTEM_NAME, 3) \
    X(VMTI_LS_VERSION, 4) \
    X(VMTI_TOTAL_TARGETS_DETECTED, 5) \
    X(VMTI_NUM_TARGETS_REPORTED, 6) \
    X(VMTI_FRAME_NUMBER, 7) \
    X(VMTI_FRAME_WIDTH, 8) \
    X(VMTI_FRAME_HEIGHT, 9) \
    X(VMTI_SOURCE_SENSOR, 10) \
    X(VMTI_HORIZONTAL_FOV, 11) \
    X(VMTI_VERTICAL_FOV, 12) \
    X(VMTI_MIIS_ID, 13) \
    X(VMTI_VTARGET_SERIES, 101) \
    X(VMTI_ALGORITHM_SERIES, 102) \
    X(VMTI_ONTOLOGY_SERIES, 103)

#define DEFINE_LOCAL_UL(name, tag) constexpr UL name = make_st_ul(ST_ID, tag);
ST0903_LOCAL_SET_TAGS(DEFINE_LOCAL_UL)
#undef DEFINE_LOCAL_UL
#undef ST0903_LOCAL_SET_TAGS

#define ST0903_VTARGET_TAGS(X) \
    X(VTARGET_CENTROID, 1) \
    X(VTARGET_BBOX_TOP_LEFT_PIXEL, 2) \
    X(VTARGET_BBOX_BOTTOM_RIGHT_PIXEL, 3) \
    X(VTARGET_PRIORITY, 4) \
    X(VTARGET_CONFIDENCE_LEVEL, 5) \
    X(VTARGET_HISTORY, 6) \
    X(VTARGET_PERCENT_TARGET_PIXELS, 7) \
    X(VTARGET_COLOR, 8) \
    X(VTARGET_INTENSITY, 9) \
    X(VTARGET_LOCATION_OFFSET_LAT, 10) \
    X(VTARGET_LOCATION_OFFSET_LON, 11) \
    X(VTARGET_LOCATION_HAE, 12) \
    X(VTARGET_BBOX_TOP_LEFT_LAT_OFFSET, 13) \
    X(VTARGET_BBOX_TOP_LEFT_LON_OFFSET, 14) \
    X(VTARGET_BBOX_BOTTOM_RIGHT_LAT_OFFSET, 15) \
    X(VTARGET_BBOX_BOTTOM_RIGHT_LON_OFFSET, 16) \
    X(VTARGET_LOCATION_PACK, 17) \
    X(VTARGET_GEOSPATIAL_CONTOUR_SERIES, 18) \
    X(VTARGET_CENTROID_ROW, 19) \
    X(VTARGET_CENTROID_COLUMN, 20) \
    X(VTARGET_FPA_INDEX_PACK, 21) \
    X(VTARGET_ALGORITHM_ID, 22) \
    X(VTARGET_DETECTION_STATUS, 23) \
    X(VTARGET_VMASK, 101) \
    X(VTARGET_VOBJECT, 102) \
    X(VTARGET_VFEATURE, 103) \
    X(VTARGET_VTRACKER, 104) \
    X(VTARGET_VCHIP, 105) \
    X(VTARGET_VCHIP_SERIES, 106) \
    X(VTARGET_VOBJECT_SERIES, 107)

#define DEFINE_VTARGET_UL(name, tag) constexpr UL name = make_st_ul(VTARGET_ST_ID, tag);
ST0903_VTARGET_TAGS(DEFINE_VTARGET_UL)
#undef DEFINE_VTARGET_UL
#undef ST0903_VTARGET_TAGS

#define ST0903_ALGORITHM_TAGS(X) \
    X(ALGORITHM_ID, 1) \
    X(ALGORITHM_NAME, 2) \
    X(ALGORITHM_VERSION, 3) \
    X(ALGORITHM_CLASS, 4) \
    X(ALGORITHM_CONFIDENCE, 5)

#define DEFINE_ALGORITHM_UL(name, tag) constexpr UL name = make_st_ul(ALGORITHM_ST_ID, tag);
ST0903_ALGORITHM_TAGS(DEFINE_ALGORITHM_UL)
#undef DEFINE_ALGORITHM_UL
#undef ST0903_ALGORITHM_TAGS

#define ST0903_ONTOLOGY_TAGS(X) \
    X(ONTOLOGY_ID, 1) \
    X(ONTOLOGY_URI, 2) \
    X(ONTOLOGY_CONFIDENCE, 3) \
    X(ONTOLOGY_FAMILY, 4)

#define DEFINE_ONTOLOGY_UL(name, tag) constexpr UL name = make_st_ul(ONTOLOGY_ST_ID, tag);
ST0903_ONTOLOGY_TAGS(DEFINE_ONTOLOGY_UL)
#undef DEFINE_ONTOLOGY_UL
#undef ST0903_ONTOLOGY_TAGS

struct VTargetPack {
    uint64_t target_id;
    KLVSet set;
};

// Register encode/decode lambdas for the above ULs
void register_st0903(KLVRegistry& reg);

// Helper to build a tag-based local set for a given ST0903 sub-set
KLVSet make_local_set(uint8_t st_id,
                      std::initializer_list<std::shared_ptr<KLVNode>> nodes);

// Helpers to build and parse the VTarget series payload (tag 101)
std::vector<uint8_t> encode_vtarget_series(const std::vector<VTargetPack>& packs);
std::vector<uint8_t> encode_vtarget_series(std::initializer_list<VTargetPack> packs);
std::vector<VTargetPack> decode_vtarget_series(const std::vector<uint8_t>& bytes);

// Helpers for algorithmSeries (tag 102)
std::vector<uint8_t> encode_algorithm_series(const std::vector<KLVSet>& sets);
std::vector<uint8_t> encode_algorithm_series(std::initializer_list<KLVSet> sets);
std::vector<KLVSet> decode_algorithm_series(const std::vector<uint8_t>& bytes);

// Helpers for ontologySeries (tag 103)
std::vector<uint8_t> encode_ontology_series(const std::vector<KLVSet>& sets);
std::vector<uint8_t> encode_ontology_series(std::initializer_list<KLVSet> sets);
std::vector<KLVSet> decode_ontology_series(const std::vector<uint8_t>& bytes);

} // namespace st0903
} // namespace misb

