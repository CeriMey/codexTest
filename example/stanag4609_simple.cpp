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
        // Timing and platform position
        KLV_ST_ITEM(0601, UNIX_TIMESTAMP, 1700000000.0),
        KLV_ST_ITEM(0601, SENSOR_LATITUDE, 48.8566),
        KLV_ST_ITEM(0601, SENSOR_LONGITUDE, 2.3522),
        KLV_ST_ITEM(0601, SENSOR_ELLIPSOID_HEIGHT, 500.0),
        KLV_ST_ITEM(0601, ALTITUDE_AGL, 480.0),
        KLV_ST_ITEM(0601, RADAR_ALTIMETER, 478.0),

        // Platform attitude and motion
        KLV_ST_ITEM(0601, PLATFORM_HEADING_ANGLE, 90.0),
        KLV_ST_ITEM(0601, PLATFORM_PITCH_ANGLE_FULL, 5.0),
        KLV_ST_ITEM(0601, PLATFORM_ROLL_ANGLE_FULL, 1.0),
        KLV_ST_ITEM(0601, PLATFORM_ANGLE_OF_ATTACK, 2.0),
        KLV_ST_ITEM(0601, PLATFORM_SIDESLIP_ANGLE, 0.5),
        KLV_ST_ITEM(0601, PLATFORM_MAGNETIC_HEADING, 89.0),
        KLV_ST_ITEM(0601, PLATFORM_COURSE_ANGLE, 88.0),
        KLV_ST_ITEM(0601, PLATFORM_GROUND_SPEED, 60.0),
        KLV_ST_ITEM(0601, PLATFORM_VERTICAL_SPEED, -1.0),
        KLV_ST_ITEM(0601, PLATFORM_FUEL_REMAINING, 80.0),
        KLV_ST_ITEM(0601, PLATFORM_STATUS, 1.0),

        // Sensor motion and geometry
        KLV_ST_ITEM(0601, SENSOR_NORTH_VELOCITY, 50.0),
        KLV_ST_ITEM(0601, SENSOR_EAST_VELOCITY, 5.0),
        KLV_ST_ITEM(0601, SENSOR_HORIZONTAL_FOV, 30.0),
        KLV_ST_ITEM(0601, SENSOR_VERTICAL_FOV, 20.0),
        KLV_ST_ITEM(0601, SENSOR_AZIMUTH_RATE, 0.1),
        KLV_ST_ITEM(0601, SENSOR_ELEVATION_RATE, 0.2),
        KLV_ST_ITEM(0601, SENSOR_ROLL_RATE, 0.3),
        KLV_ST_ITEM(0601, SENSOR_CONTROL_MODE, 1.0),
        KLV_ST_ITEM(0601, SENSOR_FRAME_RATE_PACK, 30.0),
        KLV_ST_ITEM(0601, ZOOM_PERCENTAGE, 50.0),

        // Frame center and corners
        KLV_ST_ITEM(0601, FRAME_CENTER_LATITUDE, 48.8566),
        KLV_ST_ITEM(0601, FRAME_CENTER_LONGITUDE, 2.3522),
        KLV_ST_ITEM(0601, FRAME_CENTER_ELEVATION, 50.0),
        KLV_ST_ITEM(0601, CORNER_LAT_PT1_FULL, 48.8570),
        KLV_ST_ITEM(0601, CORNER_LON_PT1_FULL, 2.3520),
        KLV_ST_ITEM(0601, CORNER_LAT_PT2_FULL, 48.8570),
        KLV_ST_ITEM(0601, CORNER_LON_PT2_FULL, 2.3530),
        KLV_ST_ITEM(0601, CORNER_LAT_PT3_FULL, 48.8560),
        KLV_ST_ITEM(0601, CORNER_LON_PT3_FULL, 2.3530),
        KLV_ST_ITEM(0601, CORNER_LAT_PT4_FULL, 48.8560),
        KLV_ST_ITEM(0601, CORNER_LON_PT4_FULL, 2.3520),

        // Environmental conditions
        KLV_ST_ITEM(0601, WIND_DIRECTION, 180.0),
        KLV_ST_ITEM(0601, WIND_SPEED, 5.0),
        KLV_ST_ITEM(0601, STATIC_PRESSURE, 1013.0),
        KLV_ST_ITEM(0601, OUTSIDE_AIR_TEMPERATURE, 15.0),
        KLV_ST_ITEM(0601, RELATIVE_HUMIDITY, 40.0),

        // Target information
        KLV_ST_ITEM(0601, TARGET_LATITUDE, 48.8571),
        KLV_ST_ITEM(0601, TARGET_LONGITUDE, 2.3540),
        KLV_ST_ITEM(0601, TARGET_LOCATION_ELEVATION, 45.0),
        KLV_ST_ITEM(0601, TARGET_WIDTH_EXTENDED, 3.0),

        // Mission timing and navigation
        KLV_ST_ITEM(0601, TIME_AIRBORNE, 3600.0),
        KLV_ST_ITEM(0601, NUMBER_OF_NAVSATS_IN_VIEW, 12.0),
        KLV_ST_ITEM(0601, POSITIONING_METHOD_SOURCE, 1.0),
        KLV_ST_ITEM(0601, LEAP_SECONDS, 37.0),
        KLV_ST_ITEM(0601, OPERATIONAL_MODE, 0.0)
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

    double ts = 0.0, lat = 0.0, lon = 0.0;
    double sensorHae = 0.0, altAgl = 0.0, radarAlt = 0.0;
    double heading = 0.0, pitch = 0.0, roll = 0.0, aoa = 0.0, sideslip = 0.0;
    double magHeading = 0.0, course = 0.0, groundSpeed = 0.0, vertSpeed = 0.0,
           fuel = 0.0, status = 0.0;
    double northVel = 0.0, eastVel = 0.0;
    double horizFov = 0.0, vertFov = 0.0;
    double azRate = 0.0, elRate = 0.0, rollRate = 0.0;
    double ctrlMode = 0.0, frameRate = 0.0, zoom = 0.0;
    double frameLat = 0.0, frameLon = 0.0, frameElev = 0.0;
    double c1Lat = 0.0, c1Lon = 0.0, c2Lat = 0.0, c2Lon = 0.0;
    double c3Lat = 0.0, c3Lon = 0.0, c4Lat = 0.0, c4Lon = 0.0;
    double windDir = 0.0, windSpd = 0.0, pressure = 0.0,
           airTemp = 0.0, humidity = 0.0;
    double tgtLat = 0.0, tgtLon = 0.0, tgtElev = 0.0, tgtWidth = 0.0;
    double timeAirborne = 0.0, navSats = 0.0, posSrc = 0.0,
           leapSecs = 0.0, opMode = 0.0;

    ST_GET(decoded, 0601, UNIX_TIMESTAMP, ts);
    ST_GET(decoded, 0601, SENSOR_LATITUDE, lat);
    ST_GET(decoded, 0601, SENSOR_LONGITUDE, lon);
    ST_GET(decoded, 0601, SENSOR_ELLIPSOID_HEIGHT, sensorHae);
    ST_GET(decoded, 0601, ALTITUDE_AGL, altAgl);
    ST_GET(decoded, 0601, RADAR_ALTIMETER, radarAlt);
    ST_GET(decoded, 0601, PLATFORM_HEADING_ANGLE, heading);
    ST_GET(decoded, 0601, PLATFORM_PITCH_ANGLE_FULL, pitch);
    ST_GET(decoded, 0601, PLATFORM_ROLL_ANGLE_FULL, roll);
    ST_GET(decoded, 0601, PLATFORM_ANGLE_OF_ATTACK, aoa);
    ST_GET(decoded, 0601, PLATFORM_SIDESLIP_ANGLE, sideslip);
    ST_GET(decoded, 0601, PLATFORM_MAGNETIC_HEADING, magHeading);
    ST_GET(decoded, 0601, PLATFORM_COURSE_ANGLE, course);
    ST_GET(decoded, 0601, PLATFORM_GROUND_SPEED, groundSpeed);
    ST_GET(decoded, 0601, PLATFORM_VERTICAL_SPEED, vertSpeed);
    ST_GET(decoded, 0601, PLATFORM_FUEL_REMAINING, fuel);
    ST_GET(decoded, 0601, PLATFORM_STATUS, status);
    ST_GET(decoded, 0601, SENSOR_NORTH_VELOCITY, northVel);
    ST_GET(decoded, 0601, SENSOR_EAST_VELOCITY, eastVel);
    ST_GET(decoded, 0601, SENSOR_HORIZONTAL_FOV, horizFov);
    ST_GET(decoded, 0601, SENSOR_VERTICAL_FOV, vertFov);
    ST_GET(decoded, 0601, SENSOR_AZIMUTH_RATE, azRate);
    ST_GET(decoded, 0601, SENSOR_ELEVATION_RATE, elRate);
    ST_GET(decoded, 0601, SENSOR_ROLL_RATE, rollRate);
    ST_GET(decoded, 0601, SENSOR_CONTROL_MODE, ctrlMode);
    ST_GET(decoded, 0601, SENSOR_FRAME_RATE_PACK, frameRate);
    ST_GET(decoded, 0601, ZOOM_PERCENTAGE, zoom);
    ST_GET(decoded, 0601, FRAME_CENTER_LATITUDE, frameLat);
    ST_GET(decoded, 0601, FRAME_CENTER_LONGITUDE, frameLon);
    ST_GET(decoded, 0601, FRAME_CENTER_ELEVATION, frameElev);
    ST_GET(decoded, 0601, CORNER_LAT_PT1_FULL, c1Lat);
    ST_GET(decoded, 0601, CORNER_LON_PT1_FULL, c1Lon);
    ST_GET(decoded, 0601, CORNER_LAT_PT2_FULL, c2Lat);
    ST_GET(decoded, 0601, CORNER_LON_PT2_FULL, c2Lon);
    ST_GET(decoded, 0601, CORNER_LAT_PT3_FULL, c3Lat);
    ST_GET(decoded, 0601, CORNER_LON_PT3_FULL, c3Lon);
    ST_GET(decoded, 0601, CORNER_LAT_PT4_FULL, c4Lat);
    ST_GET(decoded, 0601, CORNER_LON_PT4_FULL, c4Lon);
    ST_GET(decoded, 0601, WIND_DIRECTION, windDir);
    ST_GET(decoded, 0601, WIND_SPEED, windSpd);
    ST_GET(decoded, 0601, STATIC_PRESSURE, pressure);
    ST_GET(decoded, 0601, OUTSIDE_AIR_TEMPERATURE, airTemp);
    ST_GET(decoded, 0601, RELATIVE_HUMIDITY, humidity);
    ST_GET(decoded, 0601, TARGET_LATITUDE, tgtLat);
    ST_GET(decoded, 0601, TARGET_LONGITUDE, tgtLon);
    ST_GET(decoded, 0601, TARGET_LOCATION_ELEVATION, tgtElev);
    ST_GET(decoded, 0601, TARGET_WIDTH_EXTENDED, tgtWidth);
    ST_GET(decoded, 0601, TIME_AIRBORNE, timeAirborne);
    ST_GET(decoded, 0601, NUMBER_OF_NAVSATS_IN_VIEW, navSats);
    ST_GET(decoded, 0601, POSITIONING_METHOD_SOURCE, posSrc);
    ST_GET(decoded, 0601, LEAP_SECONDS, leapSecs);
    ST_GET(decoded, 0601, OPERATIONAL_MODE, opMode);

    std::cout << "Decoded UAV values:\n";
    std::cout << "  Timestamp: " << ts << '\n';
    std::cout << "  Sensor lat/lon/hae: " << lat << ", " << lon << ", "
              << sensorHae << '\n';
    std::cout << "  Altitude AGL / Radar Alt: " << altAgl << " / "
              << radarAlt << '\n';
    std::cout << "  Heading/Pitch/Roll: " << heading << " / " << pitch
              << " / " << roll << '\n';
    std::cout << "  AOA/Sideslip: " << aoa << " / " << sideslip << '\n';
    std::cout << "  Magnetic/Course: " << magHeading << " / " << course
              << '\n';
    std::cout << "  Ground/Vertical speed: " << groundSpeed << " / "
              << vertSpeed << '\n';
    std::cout << "  Fuel/Status: " << fuel << " / " << status << '\n';
    std::cout << "  Sensor velocity N/E: " << northVel << ", " << eastVel
              << '\n';
    std::cout << "  FOV H/V: " << horizFov << " / " << vertFov << '\n';
    std::cout << "  Rates az/el/roll: " << azRate << " / " << elRate
              << " / " << rollRate << '\n';
    std::cout << "  Control/FrameRate/Zoom: " << ctrlMode << " / "
              << frameRate << " / " << zoom << '\n';
    std::cout << "  Frame center lat/lon/elev: " << frameLat << ", "
              << frameLon << ", " << frameElev << '\n';
    std::cout << "  Corner1 lat/lon: " << c1Lat << ", " << c1Lon << '\n';
    std::cout << "  Corner2 lat/lon: " << c2Lat << ", " << c2Lon << '\n';
    std::cout << "  Corner3 lat/lon: " << c3Lat << ", " << c3Lon << '\n';
    std::cout << "  Corner4 lat/lon: " << c4Lat << ", " << c4Lon << '\n';
    std::cout << "  Wind dir/speed: " << windDir << " / " << windSpd
              << '\n';
    std::cout << "  Pressure/Temp/Humidity: " << pressure << " / "
              << airTemp << " / " << humidity << '\n';
    std::cout << "  Target lat/lon/elev/width: " << tgtLat << ", "
              << tgtLon << ", " << tgtElev << ", " << tgtWidth << '\n';
    std::cout << "  Time airborne: " << timeAirborne << '\n';
    std::cout << "  Nav sats/Pos src: " << navSats << " / " << posSrc
              << '\n';
    std::cout << "  Leap seconds/Op mode: " << leapSecs << " / "
              << opMode << '\n';

    KLV_GET_SET_UL(decoded, misb::st0601::VMTI_LOCAL_SET, vmti_decoded);

    std::cout << "Decoded detections:\n";
    size_t idx = 0;
    double id = 0.0, detStatus = 0.0, prob = 0.0;
    KLV_FOR_EACH_CHILD(vmti_decoded, node) {
        auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node);
        if (!leaf) continue;
        if (idx % 3 == 0) {
            id = leaf->value();
        } else if (idx % 3 == 1) {
            detStatus = leaf->value();
        } else {
            prob = leaf->value();
            std::cout << "  ID " << id
                      << " status " << detStatus
                      << " prob " << prob << '\n';
        }
        ++idx;
    }

    return 0;
}
