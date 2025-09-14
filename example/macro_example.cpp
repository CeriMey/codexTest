#include "klv_macros.h"
#include "st0601.h"
#include "st0102.h"
#include "st0903.h"
#include "stanag.h"
#include <iostream>

int main() {
    auto& reg = KLVRegistry::instance();
    misb::st0601::register_st0601(reg);
    misb::st0102::register_st0102(reg);
    misb::st0903::register_st0903(reg);

    KLVSet vmti = KLV_LOCAL_DATASET(
        KLV_TAG(misb::st0903::VMTI_TARGET_ID, 42.0),
        KLV_TAG(misb::st0903::VMTI_DETECTION_STATUS, 1.0),
        KLV_TAG(misb::st0903::VMTI_DETECTION_PROBABILITY, 0.85)
    );

    KLVSet data = KLV_LOCAL_DATASET(
        KLV_TAG(misb::st0601::SENSOR_LATITUDE, 45.0),
        KLV_TAG(misb::st0601::SENSOR_LONGITUDE, -75.0)
    );

    KLV_ADD_LEAF(data, misb::st0601::PLATFORM_HEADING_ANGLE, 90.0);
    KLV_ADD_BYTES(data, misb::st0601::VMTI_LOCAL_SET, vmti.encode());

    auto bytes = data.encode();
    std::cout << "Dataset size: " << bytes.size() << '\n';
    return 0;
}
