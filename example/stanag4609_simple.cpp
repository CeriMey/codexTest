#include "klv_macros.h"
#include "st0601.h"
#include "st0903.h"
#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>

int main() {
    auto& reg = KLVRegistry::instance();
    misb::st0601::register_st0601(reg);
    misb::st0903::register_st0903(reg);

    // Build a VMTI local set with 20 detections
    KLVSet vmti(false, misb::st0903::ST_ID, false);
    for (int i = 1; i <= 20; ++i) {
        vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_TARGET_ID, static_cast<double>(i), true));
        vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_DETECTION_STATUS, static_cast<double>(i % 2), true));
        vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_DETECTION_PROBABILITY, 0.5 + 0.01 * i, true));
    }

    // Compose the STANAG 4609 packet (UAS LS version tag added automatically)
    auto packet = STANAG4609_PACKET(
        KLV_ST_ITEM(0601, UNIX_TIMESTAMP, 1700000000.0),
        KLV_ST_ITEM(0601, SENSOR_LATITUDE, 48.8566),
        KLV_ST_ITEM(0601, SENSOR_LONGITUDE, 2.3522),
        KLV_TAG(misb::st0601::VMTI_LOCAL_SET, vmti)
    );

    std::cout << "Encoded packet:";
    for (uint8_t b : packet) {
        std::cout << ' ' << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(b);
    }
    std::cout << std::dec << '\n';

    // Decode the packet: skip the 16-byte UL and 2-byte length
    if (packet.size() < 18) return 0;
    size_t payload_len = (static_cast<size_t>(packet[16]) << 8) | packet[17];
    std::vector<uint8_t> payload(packet.begin() + 18, packet.begin() + 18 + payload_len);
    KLVSet decoded(false, misb::st0601::ST_ID, true);
    decoded.decode(payload);

    double ts = 0.0, lat = 0.0, lon = 0.0, ver = 0.0;
    ST_GET(decoded, 0601, UNIX_TIMESTAMP, ts);
    ST_GET(decoded, 0601, SENSOR_LATITUDE, lat);
    ST_GET(decoded, 0601, SENSOR_LONGITUDE, lon);
    ST_GET(decoded, 0601, UAS_LS_VERSION_NUMBER, ver);
    std::cout << "Decoded timestamp: " << ts << '\n';
    std::cout << "Decoded sensor lat/lon: " << lat << ", " << lon << '\n';
    std::cout << "Decoded UAS LS version: " << ver << '\n';

    // Extract the VMTI detections
    KLVSet vmti_decoded(false, misb::st0903::ST_ID, false);
    KLV_GET_SET(decoded, misb::st0601::VMTI_LOCAL_SET, vmti_decoded);
    std::cout << "Decoded detections:\n";
    size_t idx = 0; double id = 0, status = 0, prob = 0;
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
