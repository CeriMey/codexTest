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

    // Build a STANAG 4609 packet mixing UAV telemetry and VMTI detections
    KLVSet packet = STANAG4609_PACKET(
        KLV_ST_ITEM(0601, UNIX_TIMESTAMP, 1700000000.0),
        KLV_ST_ITEM(0601, SENSOR_LATITUDE, 48.8566),
        KLV_ST_ITEM(0601, SENSOR_LONGITUDE, 2.3522),
        KLV_ST_ITEM(0601, PLATFORM_HEADING_ANGLE, 90.0),
        KLV_ST_DATASET(0601, VMTI_LOCAL_SET,
            KLV_ST_ITEM(0903, VMTI_TARGET_ID, 1.0),
            KLV_ST_ITEM(0903, VMTI_DETECTION_STATUS, 1.0),
            KLV_ST_ITEM(0903, VMTI_DETECTION_PROBABILITY, 0.95),
            KLV_ST_ITEM(0903, VMTI_TARGET_ID, 2.0),
            KLV_ST_ITEM(0903, VMTI_DETECTION_STATUS, 1.0),
            KLV_ST_ITEM(0903, VMTI_DETECTION_PROBABILITY, 0.60)
        )
    );
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

    KLVSet vmti_decoded;
    ST_GET_SET(decoded, 0601, VMTI_LOCAL_SET, vmti_decoded);

    const auto& nodes = vmti_decoded.children();
    std::cout << "Decoded detections:\n";
    for (size_t i = 0; i + 2 < nodes.size(); i += 3) {
        auto id_leaf = std::dynamic_pointer_cast<KLVLeaf>(nodes[i]);
        auto status_leaf = std::dynamic_pointer_cast<KLVLeaf>(nodes[i + 1]);
        auto prob_leaf = std::dynamic_pointer_cast<KLVLeaf>(nodes[i + 2]);
        if (id_leaf && status_leaf && prob_leaf) {
            std::cout << "  ID " << id_leaf->value()
                      << " status " << status_leaf->value()
                      << " prob " << prob_leaf->value() << '\n';
        }
    }

    return 0;
}
