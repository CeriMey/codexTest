#include "klv.h"
#include "st0601.h"
#include "st0102.h"
#include "st0903.h"
#include "stanag.h"
#include <iostream>
#include <iomanip>
#include <tuple>
#include <vector>
#include <string>
#include <cmath>
#include <limits>

struct DetectionInfo {
    uint64_t id;
    double row;
    double column;
    double confidence; // expressed as probability [0,1]
    double status;     // enumeration per ST0903
};

static double frame_pixel_index(double row, double column, double frameWidth) {
    return column + ((row - 1.0) * frameWidth);
}

static double find_value(const KLVSet& set, const UL& ul) {
    for (const auto& node : set.children()) {
        if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node)) {
            if (leaf->ul() == ul) {
                return leaf->value();
            }
        }
    }
    return std::numeric_limits<double>::quiet_NaN();
}

int main() {
    auto& reg = KLVRegistry::instance();
    misb::st0601::register_st0601(reg);
    misb::st0102::register_st0102(reg);
    misb::st0903::register_st0903(reg);

    using NamedTag = std::tuple<std::string, UL, double>;
    std::vector<NamedTag> st0102Tags = {
        {"Classification", misb::st0102::CLASSIFICATION, 2.0},
        {"Classification system", misb::st0102::CLASSIFICATION_SYSTEM, 1.0}
    };

    auto print = [](const std::vector<uint8_t>& data) {
        for (auto b : data) {
            std::cout << std::hex << std::setw(2) << std::setfill('0')
                      << static_cast<int>(b) << ' ';
        }
        std::cout << std::dec << '\n';
    };

    const double frameWidth = 1920.0;
    const double frameHeight = 1080.0;

    std::vector<std::pair<UL, std::string>> vmtiNames = {
        {misb::st0903::VMTI_LS_VERSION, "LS version"},
        {misb::st0903::VMTI_TOTAL_TARGETS_DETECTED, "Total targets"},
        {misb::st0903::VMTI_NUM_TARGETS_REPORTED, "Targets reported"},
        {misb::st0903::VMTI_FRAME_WIDTH, "Frame width"},
        {misb::st0903::VMTI_FRAME_HEIGHT, "Frame height"},
        {misb::st0903::VMTI_HORIZONTAL_FOV, "Horizontal FoV"},
        {misb::st0903::VMTI_VERTICAL_FOV, "Vertical FoV"}
    };

    for (int frame = 0; frame < 5; ++frame) {
        double heading      = 90.0 + frame * 5.0;
        double pitch        = std::sin(frame * 0.2) * 2.0;
        double roll         = std::cos(frame * 0.3) * 1.5;
        double sensorLat    = 45.0 + 0.001 * frame;
        double sensorLon    = -75.0 + 0.001 * frame;
        double altitudeAgl  = 1000.0 + frame * 10.0;
        double groundSpeed  = 250.0 + frame * 2.0;
        double gimbalAzRate = std::sin(frame * 0.3) * 2.0;
        double gimbalElRate = std::cos(frame * 0.2) * 1.0;

        std::vector<NamedTag> st0601Tags = {
            {"Heading", misb::st0601::PLATFORM_HEADING_ANGLE, heading},
            {"Pitch", misb::st0601::PLATFORM_PITCH_ANGLE_FULL, pitch},
            {"Roll", misb::st0601::PLATFORM_ROLL_ANGLE_FULL, roll},
            {"Sensor lat", misb::st0601::SENSOR_LATITUDE, sensorLat},
            {"Sensor lon", misb::st0601::SENSOR_LONGITUDE, sensorLon},
            {"Altitude AGL", misb::st0601::ALTITUDE_AGL, altitudeAgl},
            {"Ground speed", misb::st0601::PLATFORM_GROUND_SPEED, groundSpeed},
            {"Gimbal az rate", misb::st0601::SENSOR_AZIMUTH_RATE, gimbalAzRate},
            {"Gimbal el rate", misb::st0601::SENSOR_ELEVATION_RATE, gimbalElRate}
        };

        std::vector<DetectionInfo> detections = {
            {static_cast<uint64_t>(frame * 2 + 1), 200.0 + frame * 10.0, 300.0 + frame * 8.0, 0.55 + 0.1 * std::sin(frame * 0.3), 1.0},
            {static_cast<uint64_t>(frame * 2 + 2), 150.0 + frame * 5.0, 450.0 + frame * 6.0, 0.40 + 0.15 * std::cos(frame * 0.4), 0.0}
        };

        std::vector<misb::st0903::VTargetPack> vtargetPacks;
        vtargetPacks.reserve(detections.size());
        for (const auto& detection : detections) {
            std::vector<stanag::TagValue> tags = {
                {misb::st0903::VTARGET_CENTROID, frame_pixel_index(detection.row, detection.column, frameWidth)},
                {misb::st0903::VTARGET_CENTROID_ROW, detection.row},
                {misb::st0903::VTARGET_CENTROID_COLUMN, detection.column},
                {misb::st0903::VTARGET_CONFIDENCE_LEVEL, detection.confidence},
                {misb::st0903::VTARGET_DETECTION_STATUS, detection.status}
            };
            KLVSet pack = stanag::create_dataset(tags, false);
            vtargetPacks.push_back({ detection.id, std::move(pack) });
        }

        auto vtargetSeries = misb::st0903::encode_vtarget_series(vtargetPacks);

        KLVSet vmti(false, misb::st0903::ST_ID);
        vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_LS_VERSION, 6.0, true));
        vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_TOTAL_TARGETS_DETECTED,
                                           static_cast<double>(detections.size()), true));
        vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_NUM_TARGETS_REPORTED,
                                           static_cast<double>(detections.size()), true));
        vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_FRAME_WIDTH, frameWidth, true));
        vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_FRAME_HEIGHT, frameHeight, true));
        vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_HORIZONTAL_FOV, 12.5, true));
        vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_VERTICAL_FOV, 10.0, true));
        vmti.add(std::make_shared<KLVBytes>(misb::st0903::VMTI_VTARGET_SERIES, vtargetSeries, true));

        std::vector<stanag::TagValue> vmtiValues = {
            {misb::st0903::VMTI_LS_VERSION, 6.0},
            {misb::st0903::VMTI_TOTAL_TARGETS_DETECTED, static_cast<double>(detections.size())},
            {misb::st0903::VMTI_NUM_TARGETS_REPORTED, static_cast<double>(detections.size())},
            {misb::st0903::VMTI_FRAME_WIDTH, frameWidth},
            {misb::st0903::VMTI_FRAME_HEIGHT, frameHeight}
        };
        (void)vmtiValues; // silence unused warning in case of future refactors

        KLVSet dataSet;
        for (const auto& t : st0601Tags) {
            dataSet.add(std::make_shared<KLVLeaf>(std::get<1>(t), std::get<2>(t)));
        }
        for (const auto& t : st0102Tags) {
            dataSet.add(std::make_shared<KLVLeaf>(std::get<1>(t), std::get<2>(t)));
        }
        dataSet.add(std::make_shared<KLVBytes>(misb::st0601::VMTI_LOCAL_SET, vmti.encode()));

        auto dataSetBytes = dataSet.encode();

        std::cout << "[Frame " << frame << "] Heading: " << heading
                  << ", Pitch: " << pitch
                  << ", Roll: " << roll
                  << ", Gimbal rates (az: " << gimbalAzRate
                  << ", el: " << gimbalElRate
                  << ")\n";
        print(dataSetBytes);
        KLVSet decodedSet;
        decodedSet.decode(dataSetBytes);

        std::cout << "  Decoded ST0601/ST0102 values:\n";
        KLVSet vmtiDecoded;
        auto findName = [&](const UL& ul) {
            for (const auto& t : st0601Tags) if (std::get<1>(t) == ul) return std::get<0>(t);
            for (const auto& t : st0102Tags) if (std::get<1>(t) == ul) return std::get<0>(t);
            return std::string{};
        };
        for (const auto& node : decodedSet.children()) {
            if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node)) {
                std::string name = findName(leaf->ul());
                if (!name.empty()) {
                    std::cout << "    " << name << ": " << leaf->value() << '\n';
                }
            } else if (auto bytesNode = std::dynamic_pointer_cast<KLVBytes>(node)) {
                if (bytesNode->ul() == misb::st0601::VMTI_LOCAL_SET) {
                    vmtiDecoded.decode(bytesNode->value());
                }
            }
        }

        std::cout << "  Decoded ST0903 values:\n";
        for (const auto& node : vmtiDecoded.children()) {
            if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node)) {
                for (const auto& entry : vmtiNames) {
                    if (leaf->ul() == entry.first) {
                        std::cout << "    " << entry.second << ": " << leaf->value() << '\n';
                    }
                }
            } else if (auto bytesNode = std::dynamic_pointer_cast<KLVBytes>(node)) {
                if (bytesNode->ul() == misb::st0903::VMTI_VTARGET_SERIES) {
                    auto decodedPacks = misb::st0903::decode_vtarget_series(bytesNode->value());
                    for (const auto& pack : decodedPacks) {
                        double centroidIndex = find_value(pack.set, misb::st0903::VTARGET_CENTROID);
                        double centroidRow = find_value(pack.set, misb::st0903::VTARGET_CENTROID_ROW);
                        double centroidCol = find_value(pack.set, misb::st0903::VTARGET_CENTROID_COLUMN);
                        double confidence = find_value(pack.set, misb::st0903::VTARGET_CONFIDENCE_LEVEL);
                        double status = find_value(pack.set, misb::st0903::VTARGET_DETECTION_STATUS);
                        std::cout << "    Target " << pack.target_id
                                  << ": centroid index " << centroidIndex
                                  << ", row " << centroidRow
                                  << ", column " << centroidCol
                                  << ", confidence " << confidence
                                  << ", status " << status << '\n';
                    }
                }
            }
        }
    }

    return 0;
}

