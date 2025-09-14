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

int main() {
    auto& reg = KLVRegistry::instance();
    misb::st0601::register_st0601(reg);
    misb::st0102::register_st0102(reg);
    misb::st0903::register_st0903(reg);

    using NamedTag = std::tuple<std::string, UL, double>;
    std::vector<NamedTag> st0601Tags = {
        {"Heading", misb::st0601::PLATFORM_HEADING_ANGLE, 90.0},
        {"Elevation", misb::st0601::TARGET_LOCATION_ELEVATION, 1000.0},
        {"Gate width", misb::st0601::TARGET_TRACK_GATE_WIDTH, 256.0},
        {"Gate height", misb::st0601::TARGET_TRACK_GATE_HEIGHT, 128.0},
        {"Gate X", misb::st0601::TARGET_TRACK_GATE_X, 640.0},
        {"Gate Y", misb::st0601::TARGET_TRACK_GATE_Y, 480.0},
        {"Sensor lat", misb::st0601::SENSOR_LATITUDE, 45.0},
        {"Sensor lon", misb::st0601::SENSOR_LONGITUDE, -75.0},
        {"Horizontal FOV", misb::st0601::SENSOR_HORIZONTAL_FOV, 20.0},
        {"Vertical FOV", misb::st0601::SENSOR_VERTICAL_FOV, 15.0},
        {"Frame center lat", misb::st0601::FRAME_CENTER_LATITUDE, 44.999},
        {"Frame center lon", misb::st0601::FRAME_CENTER_LONGITUDE, -74.999},
        {"Frame center elev", misb::st0601::FRAME_CENTER_ELEVATION, 1100.0},
        {"Offset corner1 lat", misb::st0601::OFFSET_CORNER_LAT_PT1, 0.01},
        {"Offset corner1 lon", misb::st0601::OFFSET_CORNER_LON_PT1, -0.02},
        {"Wind direction", misb::st0601::WIND_DIRECTION, 270.0},
        {"Wind speed", misb::st0601::WIND_SPEED, 10.0},
        {"Weapon load", misb::st0601::WEAPON_LOAD, 0x1234},
        {"Magnetic heading", misb::st0601::PLATFORM_MAGNETIC_HEADING, 180.0},
        {"Sensor north velocity", misb::st0601::SENSOR_NORTH_VELOCITY, 30.0}
    };

    std::vector<NamedTag> st0102Tags = {
        {"Classification", misb::st0102::CLASSIFICATION, 2.0},
        {"Classification system", misb::st0102::CLASSIFICATION_SYSTEM, 1.0}
    };

    std::vector<NamedTag> st0903Tags = {
        {"Target ID", misb::st0903::VMTI_TARGET_ID, 42.0},
        {"Detection status", misb::st0903::VMTI_DETECTION_STATUS, 1.0},
        {"Detection probability", misb::st0903::VMTI_DETECTION_PROBABILITY, 0.85}
    };

    // Build ST0903 local set then embed it using ST0601 tag 74
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

    auto print = [](const std::vector<uint8_t>& data) {
        for (auto b : data) {
            std::cout << std::hex << std::setw(2) << std::setfill('0')
                      << static_cast<int>(b) << ' ';
        }
        std::cout << std::dec << '\n';
    };

    std::cout << "STANAG 4609 dataset bytes: ";
    print(dataSetBytes);

    for (const auto& t : st0601Tags) {
        KLVLeaf leaf(std::get<1>(t), std::get<2>(t));
        auto bytes = leaf.encode();
        KLVLeaf decoded(std::get<1>(t));
        decoded.decode(bytes);
        std::cout << "Decoded " << std::get<0>(t) << ": " << decoded.value() << '\n';
    }

    for (const auto& t : st0102Tags) {
        KLVLeaf leaf(std::get<1>(t), std::get<2>(t));
        auto bytes = leaf.encode();
        KLVLeaf decoded(std::get<1>(t));
        decoded.decode(bytes);
        std::cout << "Decoded " << std::get<0>(t) << ": " << decoded.value() << '\n';
    }

    for (const auto& t : st0903Tags) {
        KLVLeaf leaf(std::get<1>(t), std::get<2>(t));
        auto bytes = leaf.encode();
        KLVLeaf decoded(std::get<1>(t));
        decoded.decode(bytes);
        std::cout << "Decoded " << std::get<0>(t) << ": " << decoded.value() << '\n';
    }

    return 0;
}
