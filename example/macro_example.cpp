#include "klv_macros.h"
#include "st0601.h"
#include "st0102.h"
#include "st0903.h"
#include "stanag.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>

struct Detection {
    uint64_t id;
    double row;
    double column;
    double confidence;
    double status;
};

static double compute_pixel(double row, double column, double frameWidth) {
    return column + ((row - 1.0) * frameWidth);
}

static double extract_value(const KLVSet& set, const UL& ul) {
    for (const auto& node : set.children()) {
        if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node)) {
            if (leaf->ul() == ul) return leaf->value();
        }
    }
    return std::numeric_limits<double>::quiet_NaN();
}

int main() {
    auto& reg = KLVRegistry::instance();
    misb::st0601::register_st0601(reg);
    misb::st0102::register_st0102(reg);
    misb::st0903::register_st0903(reg);

    const double frameWidth = 1280.0;
    const double frameHeight = 720.0;

    std::vector<Detection> detections = {
        {1, 120.0, 300.0, 0.95, 1.0},
        {2, 256.0, 512.0, 0.72, 1.0},
        {3, 400.0, 640.0, 0.55, 0.0},
        {4, 500.0, 850.0, 0.40, 0.0},
        {5, 620.0, 900.0, 0.25, 0.0}
    };

    std::cout << "Input UAV data:" << std::fixed << std::setprecision(2) << '\n';
    std::cout << "  Sensor Latitude: 45.00\n";
    std::cout << "  Sensor Longitude: -75.00\n";
    std::cout << "  Platform Heading: 90.00\n";

    std::cout << "Detections:" << '\n';
    std::vector<misb::st0903::VTargetPack> packs;
    for (const auto& d : detections) {
        std::cout << "  ID " << d.id
                  << " centroid (row,col)=(" << d.row << ", " << d.column << ")"
                  << " confidence " << d.confidence
                  << " status " << d.status << '\n';
        std::vector<stanag::TagValue> entries = {
            {misb::st0903::VTARGET_CENTROID, compute_pixel(d.row, d.column, frameWidth)},
            {misb::st0903::VTARGET_CENTROID_ROW, d.row},
            {misb::st0903::VTARGET_CENTROID_COLUMN, d.column},
            {misb::st0903::VTARGET_CONFIDENCE_LEVEL, d.confidence},
            {misb::st0903::VTARGET_DETECTION_STATUS, d.status}
        };
        packs.push_back({ d.id, stanag::create_dataset(entries, false) });
    }

    auto series = misb::st0903::encode_vtarget_series(packs);

    KLVSet vmti(false, misb::st0903::ST_ID);
    vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_LS_VERSION, 6.0, true));
    vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_TOTAL_TARGETS_DETECTED,
                                       static_cast<double>(detections.size()), true));
    vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_NUM_TARGETS_REPORTED,
                                       static_cast<double>(detections.size()), true));
    vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_FRAME_WIDTH, frameWidth, true));
    vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_FRAME_HEIGHT, frameHeight, true));
    vmti.add(std::make_shared<KLVBytes>(misb::st0903::VMTI_VTARGET_SERIES, series, true));

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

    std::cout << "Decoded detections:" << '\n';
    for (const auto& node : vmti_decoded.children()) {
        if (auto bytesNode = std::dynamic_pointer_cast<KLVBytes>(node)) {
            if (bytesNode->ul() == misb::st0903::VMTI_VTARGET_SERIES) {
                auto decodedPacks = misb::st0903::decode_vtarget_series(bytesNode->value());
                for (const auto& pack : decodedPacks) {
                    double centroid = extract_value(pack.set, misb::st0903::VTARGET_CENTROID);
                    double row = extract_value(pack.set, misb::st0903::VTARGET_CENTROID_ROW);
                    double col = extract_value(pack.set, misb::st0903::VTARGET_CENTROID_COLUMN);
                    double conf = extract_value(pack.set, misb::st0903::VTARGET_CONFIDENCE_LEVEL);
                    double status = extract_value(pack.set, misb::st0903::VTARGET_DETECTION_STATUS);
                    std::cout << "  ID " << pack.target_id
                              << " centroid " << centroid
                              << " (row,col)=(" << row << ", " << col << ")"
                              << " confidence " << conf
                              << " status " << status << '\n';
                }
            }
        }
    }

    return 0;
}

