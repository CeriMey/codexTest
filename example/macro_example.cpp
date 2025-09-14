#include "klv_macros.h"
#include "st0601.h"
#include "st0102.h"
#include "st0903.h"
#include "stanag.h"
#include <iostream>
#include <iomanip>
#include <vector>

struct Detection {
    double id;
    double prob;
    double class_id;
    double tracker_id;
};

int main() {
    auto& reg = KLVRegistry::instance();
    misb::st0601::register_st0601(reg);
    misb::st0102::register_st0102(reg);
    misb::st0903::register_st0903(reg);

    std::vector<Detection> detections = {
        {1, 0.95, 2, 1001},
        {2, 0.80, 3, 1002},
        {3, 0.60, 1, 1003},
        {4, 0.40, 5, 1004},
        {5, 0.20, 4, 1005}
    };

    std::cout << "Input UAV data:" << std::fixed << std::setprecision(2) << '\n';
    std::cout << "  Sensor Latitude: 45.00\n";
    std::cout << "  Sensor Longitude: -75.00\n";
    std::cout << "  Platform Heading: 90.00\n";

    std::cout << "Detections:" << '\n';
    KLVSet vmti;
    for (const auto& d : detections) {
        std::cout << "  ID " << d.id
                  << " class " << d.class_id
                  << " tracker " << d.tracker_id
                  << " prob " << d.prob << '\n';
        KLV_ADD_LEAF(vmti, misb::st0903::VMTI_TARGET_ID, d.id);
        KLV_ADD_LEAF(vmti, misb::st0903::VMTI_CLASS_ID, d.class_id);
        KLV_ADD_LEAF(vmti, misb::st0903::VMTI_TRACKER_ID, d.tracker_id);
        KLV_ADD_LEAF(vmti, misb::st0903::VMTI_DETECTION_PROBABILITY, d.prob);
    }

    KLVSet data = KLV_LOCAL_DATASET(
        KLV_TAG(misb::st0601::SENSOR_LATITUDE, 45.0),
        KLV_TAG(misb::st0601::SENSOR_LONGITUDE, -75.0),
        KLV_TAG(misb::st0601::PLATFORM_HEADING_ANGLE, 90.0)
    );
    KLV_ADD_BYTES(data, misb::st0601::VMTI_LOCAL_SET, vmti.encode());

    auto bytes = data.encode();
    std::cout << "\nEncoded packet:";
    for (uint8_t b : bytes) {
        std::cout << ' ' << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(b);
    }
    std::cout << std::dec << '\n';

    // Decode
    KLVSet decoded;
    decoded.decode(bytes);
    std::cout << "\nDecoded UAV data:" << '\n';
    KLVSet vmti_decoded;
    for (const auto& node : decoded.children()) {
        if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node)) {
            const UL& ul = leaf->ul();
            if (ul == misb::st0601::SENSOR_LATITUDE) {
                std::cout << "  Sensor Latitude: " << leaf->value() << '\n';
            } else if (ul == misb::st0601::SENSOR_LONGITUDE) {
                std::cout << "  Sensor Longitude: " << leaf->value() << '\n';
            } else if (ul == misb::st0601::PLATFORM_HEADING_ANGLE) {
                std::cout << "  Platform Heading: " << leaf->value() << '\n';
            }
        } else if (auto bytesNode = std::dynamic_pointer_cast<KLVBytes>(node)) {
            if (bytesNode->ul() == misb::st0601::VMTI_LOCAL_SET) {
                vmti_decoded.decode(bytesNode->value());
            }
        }
    }

    const auto& nodes = vmti_decoded.children();
    std::cout << "Decoded Detections:" << '\n';
    for (size_t i = 0; i + 3 < nodes.size(); i += 4) {
        auto id_leaf = std::dynamic_pointer_cast<KLVLeaf>(nodes[i]);
        auto class_leaf = std::dynamic_pointer_cast<KLVLeaf>(nodes[i + 1]);
        auto tracker_leaf = std::dynamic_pointer_cast<KLVLeaf>(nodes[i + 2]);
        auto prob_leaf = std::dynamic_pointer_cast<KLVLeaf>(nodes[i + 3]);
        if (id_leaf && class_leaf && tracker_leaf && prob_leaf) {
            std::cout << "  ID " << id_leaf->value()
                      << " class " << class_leaf->value()
                      << " tracker " << tracker_leaf->value()
                      << " prob " << prob_leaf->value() << '\n';
        }
    }

    return 0;
}
