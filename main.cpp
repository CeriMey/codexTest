#include "klv.h"
#include "st0601.h"
#include <iostream>
#include <iomanip>

int main() {
    auto& reg = KLVRegistry::instance();
    misb::st0601::register_st0601(reg);

    KLVLeaf heading(misb::st0601::PLATFORM_HEADING_ANGLE, 90.0);
    KLVLeaf elevation(misb::st0601::TARGET_LOCATION_ELEVATION, 1000.0);
    KLVLeaf gateWidth(misb::st0601::TARGET_TRACK_GATE_WIDTH, 256.0);

    auto headingBytes = heading.encode();
    auto elevationBytes = elevation.encode();
    auto widthBytes = gateWidth.encode();

    auto print = [](const std::vector<uint8_t>& data) {
        for (auto b : data) {
            std::cout << std::hex << std::setw(2) << std::setfill('0')
                      << static_cast<int>(b) << ' ';
        }
        std::cout << std::dec << '\n';
    };

    std::cout << "Heading bytes: ";
    print(headingBytes);
    std::cout << "Elevation bytes: ";
    print(elevationBytes);
    std::cout << "Gate width bytes: ";
    print(widthBytes);

    KLVLeaf decodedHeading(misb::st0601::PLATFORM_HEADING_ANGLE);
    decodedHeading.decode(headingBytes);
    KLVLeaf decodedElevation(misb::st0601::TARGET_LOCATION_ELEVATION);
    decodedElevation.decode(elevationBytes);
    KLVLeaf decodedWidth(misb::st0601::TARGET_TRACK_GATE_WIDTH);
    decodedWidth.decode(widthBytes);

    std::cout << "Decoded heading: " << decodedHeading.value() << '\n';
    std::cout << "Decoded elevation: " << decodedElevation.value() << '\n';
    std::cout << "Decoded gate width: " << decodedWidth.value() << '\n';

    return 0;
}
