#include "klv_macros.h"
#include "st0601.h"
#include "st0903.h"
#include <iostream>
#include <iomanip>
#include <memory>

int main() {
    auto& reg = KLVRegistry::instance();
    misb::st0601::register_st0601(reg);
    misb::st0903::register_st0903(reg);

    // Build a STANAG 4609 packet mixing UAV telemetry and 20 VMTI detections
    KLVSet packet = STANAG4609_PACKET(
        KLV_ST_ITEM(0601, UNIX_TIMESTAMP, 1700000000.0),
        KLV_ST_ITEM(0601, SENSOR_LATITUDE, 48.8566),
        KLV_ST_ITEM(0601, SENSOR_LONGITUDE, 2.3522),
        KLV_ST_ITEM(0601, PLATFORM_HEADING_ANGLE, 90.0)
    );

    // Add at least 60 values: 20 detections (ID, status, probability)
    KLVSet vmti;
    for (int i = 1; i <= 20; ++i) {
        KLV_ADD_LEAF(vmti, misb::st0903::VMTI_TARGET_ID, static_cast<double>(i));
        KLV_ADD_LEAF(vmti, misb::st0903::VMTI_DETECTION_STATUS, static_cast<double>(i % 2));
        KLV_ADD_LEAF(vmti, misb::st0903::VMTI_DETECTION_PROBABILITY, 0.5 + 0.01 * i);
    }
    KLV_ADD_BYTES(packet, misb::st0601::VMTI_LOCAL_SET, vmti.encode());
    auto bytes = packet.encode();

    std::cout << "Encoded packet:";
    for (uint8_t b : bytes) {
        std::cout << ' ' << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(b);
    }
    std::cout << std::dec << '\n';

    // Decode the packet
    KLVSet decoded;
    KLV_DECODE_SET(decoded, bytes);

    double ts = 0.0, lat = 0.0, lon = 0.0, heading = 0.0;
    ST_GET(decoded, 0601, UNIX_TIMESTAMP, ts);
    ST_GET(decoded, 0601, SENSOR_LATITUDE, lat);
    ST_GET(decoded, 0601, SENSOR_LONGITUDE, lon);
    ST_GET(decoded, 0601, PLATFORM_HEADING_ANGLE, heading);

    std::cout << "Decoded UAV values:\n";
    std::cout << "  Timestamp: " << ts << '\n';
    std::cout << "  Latitude : " << lat << '\n';
    std::cout << "  Longitude: " << lon << '\n';
    std::cout << "  Heading  : " << heading << '\n';

    KLV_GET_SET_UL(decoded, misb::st0601::VMTI_LOCAL_SET, vmti_decoded);

    std::cout << "Decoded detections:\n";
    size_t idx = 0;
    double id = 0.0, status = 0.0, prob = 0.0;
    KLV_FOR_EACH_CHILD(vmti_decoded, node) {
        auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node);
        if (!leaf) continue;
        if (idx % 3 == 0) {
            id = leaf->value();
        } else if (idx % 3 == 1) {
            status = leaf->value();
        } else {
            prob = leaf->value();
            std::cout << "  ID " << id
                      << " status " << status
                      << " prob " << prob << '\n';
        }
        ++idx;
    }

    return 0;
}
