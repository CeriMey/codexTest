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
    KLVLeaf gateHeight(misb::st0601::TARGET_TRACK_GATE_HEIGHT, 128.0);
    KLVLeaf gateX(misb::st0601::TARGET_TRACK_GATE_X, 640.0);
    KLVLeaf gateY(misb::st0601::TARGET_TRACK_GATE_Y, 480.0);
    KLVLeaf sensorLat(misb::st0601::SENSOR_LATITUDE, 45.0);
    KLVLeaf sensorLon(misb::st0601::SENSOR_LONGITUDE, -75.0);
    KLVLeaf hFov(misb::st0601::SENSOR_HORIZONTAL_FOV, 20.0);
    KLVLeaf vFov(misb::st0601::SENSOR_VERTICAL_FOV, 15.0);
    KLVLeaf frameLat(misb::st0601::FRAME_CENTER_LATITUDE, 44.999);
    KLVLeaf frameLon(misb::st0601::FRAME_CENTER_LONGITUDE, -74.999);
    KLVLeaf frameElev(misb::st0601::FRAME_CENTER_ELEVATION, 1100.0);
    KLVLeaf offLat1(misb::st0601::OFFSET_CORNER_LAT_PT1, 0.01);
    KLVLeaf offLon1(misb::st0601::OFFSET_CORNER_LON_PT1, -0.02);
    KLVLeaf windDir(misb::st0601::WIND_DIRECTION, 270.0);
    KLVLeaf windSpd(misb::st0601::WIND_SPEED, 10.0);
    KLVLeaf weaponLoad(misb::st0601::WEAPON_LOAD, 0x1234);
    KLVLeaf magHeading(misb::st0601::PLATFORM_MAGNETIC_HEADING, 180.0);
    KLVLeaf northVel(misb::st0601::SENSOR_NORTH_VELOCITY, 30.0);

    auto headingBytes = heading.encode();
    auto elevationBytes = elevation.encode();
    auto widthBytes = gateWidth.encode();
    auto heightBytes = gateHeight.encode();
    auto xBytes = gateX.encode();
    auto yBytes = gateY.encode();
    auto sLatBytes = sensorLat.encode();
    auto sLonBytes = sensorLon.encode();
    auto hFovBytes = hFov.encode();
    auto vFovBytes = vFov.encode();
    auto fLatBytes = frameLat.encode();
    auto fLonBytes = frameLon.encode();
    auto fElevBytes = frameElev.encode();
    auto offLat1Bytes = offLat1.encode();
    auto offLon1Bytes = offLon1.encode();
    auto windDirBytes = windDir.encode();
    auto windSpdBytes = windSpd.encode();
    auto weaponLoadBytes = weaponLoad.encode();
    auto magHeadingBytes = magHeading.encode();
    auto northVelBytes = northVel.encode();

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
    std::cout << "Gate height bytes: ";
    print(heightBytes);
    std::cout << "Gate X bytes: ";
    print(xBytes);
    std::cout << "Gate Y bytes: ";
    print(yBytes);
    std::cout << "Sensor lat bytes: ";
    print(sLatBytes);
    std::cout << "Sensor lon bytes: ";
    print(sLonBytes);
    std::cout << "Horizontal FOV bytes: ";
    print(hFovBytes);
    std::cout << "Vertical FOV bytes: ";
    print(vFovBytes);
    std::cout << "Frame center lat bytes: ";
    print(fLatBytes);
    std::cout << "Frame center lon bytes: ";
    print(fLonBytes);
    std::cout << "Frame center elev bytes: ";
    print(fElevBytes);
    std::cout << "Offset corner1 lat bytes: ";
    print(offLat1Bytes);
    std::cout << "Offset corner1 lon bytes: ";
    print(offLon1Bytes);
    std::cout << "Wind direction bytes: ";
    print(windDirBytes);
    std::cout << "Wind speed bytes: ";
    print(windSpdBytes);
    std::cout << "Weapon load bytes: ";
    print(weaponLoadBytes);
    std::cout << "Magnetic heading bytes: ";
    print(magHeadingBytes);
    std::cout << "Sensor north velocity bytes: ";
    print(northVelBytes);

    KLVLeaf decodedHeading(misb::st0601::PLATFORM_HEADING_ANGLE);
    decodedHeading.decode(headingBytes);
    KLVLeaf decodedElevation(misb::st0601::TARGET_LOCATION_ELEVATION);
    decodedElevation.decode(elevationBytes);
    KLVLeaf decodedWidth(misb::st0601::TARGET_TRACK_GATE_WIDTH);
    decodedWidth.decode(widthBytes);
    KLVLeaf decodedHeight(misb::st0601::TARGET_TRACK_GATE_HEIGHT);
    decodedHeight.decode(heightBytes);
    KLVLeaf decodedX(misb::st0601::TARGET_TRACK_GATE_X);
    decodedX.decode(xBytes);
    KLVLeaf decodedY(misb::st0601::TARGET_TRACK_GATE_Y);
    decodedY.decode(yBytes);
    KLVLeaf decodedSLat(misb::st0601::SENSOR_LATITUDE);
    decodedSLat.decode(sLatBytes);
    KLVLeaf decodedSLon(misb::st0601::SENSOR_LONGITUDE);
    decodedSLon.decode(sLonBytes);
    KLVLeaf decodedHFov(misb::st0601::SENSOR_HORIZONTAL_FOV);
    decodedHFov.decode(hFovBytes);
    KLVLeaf decodedVFov(misb::st0601::SENSOR_VERTICAL_FOV);
    decodedVFov.decode(vFovBytes);
    KLVLeaf decodedFLat(misb::st0601::FRAME_CENTER_LATITUDE);
    decodedFLat.decode(fLatBytes);
    KLVLeaf decodedFLon(misb::st0601::FRAME_CENTER_LONGITUDE);
    decodedFLon.decode(fLonBytes);
    KLVLeaf decodedFElev(misb::st0601::FRAME_CENTER_ELEVATION);
    decodedFElev.decode(fElevBytes);
    KLVLeaf decodedOffLat1(misb::st0601::OFFSET_CORNER_LAT_PT1);
    decodedOffLat1.decode(offLat1Bytes);
    KLVLeaf decodedOffLon1(misb::st0601::OFFSET_CORNER_LON_PT1);
    decodedOffLon1.decode(offLon1Bytes);
    KLVLeaf decodedWindDir(misb::st0601::WIND_DIRECTION);
    decodedWindDir.decode(windDirBytes);
    KLVLeaf decodedWindSpd(misb::st0601::WIND_SPEED);
    decodedWindSpd.decode(windSpdBytes);
    KLVLeaf decodedWeaponLoad(misb::st0601::WEAPON_LOAD);
    decodedWeaponLoad.decode(weaponLoadBytes);
    KLVLeaf decodedMagHeading(misb::st0601::PLATFORM_MAGNETIC_HEADING);
    decodedMagHeading.decode(magHeadingBytes);
    KLVLeaf decodedNorthVel(misb::st0601::SENSOR_NORTH_VELOCITY);
    decodedNorthVel.decode(northVelBytes);

    std::cout << "Decoded heading: " << decodedHeading.value() << '\n';
    std::cout << "Decoded elevation: " << decodedElevation.value() << '\n';
    std::cout << "Decoded gate width: " << decodedWidth.value() << '\n';
    std::cout << "Decoded gate height: " << decodedHeight.value() << '\n';
    std::cout << "Decoded gate X: " << decodedX.value() << '\n';
    std::cout << "Decoded gate Y: " << decodedY.value() << '\n';
    std::cout << "Decoded sensor lat: " << decodedSLat.value() << '\n';
    std::cout << "Decoded sensor lon: " << decodedSLon.value() << '\n';
    std::cout << "Decoded horizontal FOV: " << decodedHFov.value() << '\n';
    std::cout << "Decoded vertical FOV: " << decodedVFov.value() << '\n';
    std::cout << "Decoded frame center lat: " << decodedFLat.value() << '\n';
    std::cout << "Decoded frame center lon: " << decodedFLon.value() << '\n';
    std::cout << "Decoded frame center elev: " << decodedFElev.value() << '\n';
    std::cout << "Decoded offset corner1 lat: " << decodedOffLat1.value() << '\n';
    std::cout << "Decoded offset corner1 lon: " << decodedOffLon1.value() << '\n';
    std::cout << "Decoded wind direction: " << decodedWindDir.value() << '\n';
    std::cout << "Decoded wind speed: " << decodedWindSpd.value() << '\n';
    std::cout << "Decoded weapon load: " << decodedWeaponLoad.value() << '\n';
    std::cout << "Decoded magnetic heading: " << decodedMagHeading.value() << '\n';
    std::cout << "Decoded sensor north velocity: " << decodedNorthVel.value() << '\n';

    return 0;
}
