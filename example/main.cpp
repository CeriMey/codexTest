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

    for (int frame = 0; frame < 5; ++frame) {
        double heading = 90.0 + frame * 5.0;
        double sensorLat = 45.0 + 0.001 * frame;
        double sensorLon = -75.0 + 0.001 * frame;
        double gimbalAzRate = std::sin(frame * 0.3) * 2.0;
        double detectionProb = 0.5 + 0.4 * std::sin(frame * 0.5);
        double detectionStatus = detectionProb > 0.6 ? 1.0 : 0.0;

        std::vector<NamedTag> st0601Tags = {
            {"Heading", misb::st0601::PLATFORM_HEADING_ANGLE, heading},
            {"Sensor lat", misb::st0601::SENSOR_LATITUDE, sensorLat},
            {"Sensor lon", misb::st0601::SENSOR_LONGITUDE, sensorLon},
            {"Gimbal az rate", misb::st0601::SENSOR_AZIMUTH_RATE, gimbalAzRate}
        };

        std::vector<NamedTag> st0903Tags = {
            {"Target ID", misb::st0903::VMTI_TARGET_ID, 42.0},
            {"Detection status", misb::st0903::VMTI_DETECTION_STATUS, detectionStatus},
            {"Detection probability", misb::st0903::VMTI_DETECTION_PROBABILITY, detectionProb}
        };

        std::vector<stanag::TagValue> vmtiValues;
        for (const auto& t : st0903Tags) {
            vmtiValues.push_back({std::get<1>(t), std::get<2>(t)});
        }
        KLVSet vmtiSet = stanag::create_dataset(vmtiValues);

        KLVSet dataSet;
        for (const auto& t : st0601Tags) {
            dataSet.add(std::make_shared<KLVLeaf>(std::get<1>(t), std::get<2>(t)));
        }
        for (const auto& t : st0102Tags) {
            dataSet.add(std::make_shared<KLVLeaf>(std::get<1>(t), std::get<2>(t)));
        }
        dataSet.add(std::make_shared<KLVBytes>(misb::st0601::VMTI_LOCAL_SET, vmtiSet.encode()));

        auto dataSetBytes = dataSet.encode();

        std::cout << "[Frame " << frame << "] Heading: " << heading
                  << ", Gimbal az rate: " << gimbalAzRate
                  << ", Detection prob: " << detectionProb << '\n';
        print(dataSetBytes);

        for (const auto& t : st0903Tags) {
            KLVLeaf leaf(std::get<1>(t), std::get<2>(t));
            auto bytes = leaf.encode();
            KLVLeaf decoded(std::get<1>(t));
            decoded.decode(bytes);
            std::cout << "  Decoded " << std::get<0>(t)
                      << ": " << decoded.value() << '\n';
        }
    }

    return 0;
}

