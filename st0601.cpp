#include "st0601.h"

namespace misb {
namespace st0601 {

// Dummy UL values for demonstration purposes
const UL PLATFORM_HEADING_ANGLE   = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00};
const UL TARGET_LOCATION_ELEVATION = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01};
const UL TARGET_TRACK_GATE_WIDTH   = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x02};
const UL TARGET_TRACK_GATE_HEIGHT  = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x03};
const UL TARGET_TRACK_GATE_X       = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x04};
const UL TARGET_TRACK_GATE_Y       = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x05};
const UL SENSOR_LATITUDE           = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x06};
const UL SENSOR_LONGITUDE          = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x07};
const UL SENSOR_HORIZONTAL_FOV     = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x08};
const UL SENSOR_VERTICAL_FOV       = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x09};
const UL FRAME_CENTER_LATITUDE     = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x0A};
const UL FRAME_CENTER_LONGITUDE    = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x0B};
const UL FRAME_CENTER_ELEVATION    = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x0C};
const UL OFFSET_CORNER_LAT_PT1     = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x0D};
const UL OFFSET_CORNER_LON_PT1     = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x0E};
const UL OFFSET_CORNER_LAT_PT2     = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x0F};
const UL OFFSET_CORNER_LON_PT2     = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x10};
const UL OFFSET_CORNER_LAT_PT3     = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x11};
const UL OFFSET_CORNER_LON_PT3     = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x12};
const UL OFFSET_CORNER_LAT_PT4     = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x13};
const UL OFFSET_CORNER_LON_PT4     = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x14};
const UL ICING_DETECTED            = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x15};
const UL WIND_DIRECTION            = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x16};
const UL WIND_SPEED                = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x17};
const UL STATIC_PRESSURE           = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x18};
const UL DENSITY_ALTITUDE          = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x19};
const UL OUTSIDE_AIR_TEMPERATURE   = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x1A};
const UL TARGET_LATITUDE           = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x1B};
const UL TARGET_LONGITUDE          = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x1C};
const UL TARGET_ERROR_CE90         = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x1E};
const UL TARGET_ERROR_LE90         = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x1F};
const UL GENERIC_FLAG_DATA01       = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x20};
const UL DIFFERENTIAL_PRESSURE     = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x22};
const UL PLATFORM_ANGLE_OF_ATTACK  = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x23};
const UL PLATFORM_VERTICAL_SPEED   = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x24};
const UL PLATFORM_SIDESLIP_ANGLE   = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x25};
const UL AIRFIELD_BAROMETRIC_PRESSURE = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x26};
const UL AIRFIELD_ELEVATION        = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x27};
const UL RELATIVE_HUMIDITY         = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x28};
const UL PLATFORM_GROUND_SPEED     = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x29};
const UL GROUND_RANGE              = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x2A};
const UL PLATFORM_FUEL_REMAINING   = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x2B};
const UL WEAPON_LOAD               = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x2C};
const UL WEAPON_FIRED              = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x2D};
const UL LASER_PRF_CODE            = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x2E};
const UL SENSOR_FOV_NAME           = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x2F};
const UL PLATFORM_MAGNETIC_HEADING = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x30};
const UL UAS_LS_VERSION_NUMBER     = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x31};
const UL ALTERNATE_PLATFORM_LATITUDE  = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x33};
const UL ALTERNATE_PLATFORM_LONGITUDE = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x34};
const UL ALTERNATE_PLATFORM_ALTITUDE  = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x35};
const UL ALTERNATE_PLATFORM_HEADING   = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x37};
const UL EVENT_START_TIME_UTC      = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x38};
const UL SENSOR_ELLIPSOID_HEIGHT   = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x3B};
const UL ALTERNATE_PLATFORM_ELLIPSOID_HEIGHT = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x3C};
const UL OPERATIONAL_MODE          = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x3D};
const UL FRAME_CENTER_HAE          = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x3E};
const UL SENSOR_NORTH_VELOCITY     = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x3F};
const UL SENSOR_EAST_VELOCITY      = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x40};
const UL CORNER_LAT_PT1_FULL       = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x42};
const UL CORNER_LON_PT1_FULL       = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x43};
const UL CORNER_LAT_PT2_FULL       = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x44};
const UL CORNER_LON_PT2_FULL       = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x45};
const UL CORNER_LAT_PT3_FULL       = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x46};
const UL CORNER_LON_PT3_FULL       = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x47};
const UL CORNER_LAT_PT4_FULL       = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x48};
const UL CORNER_LON_PT4_FULL       = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x49};
const UL PLATFORM_PITCH_ANGLE_FULL = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x4A};
const UL PLATFORM_ROLL_ANGLE_FULL  = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x4B};
const UL PLATFORM_AOA_FULL         = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x4C};
const UL PLATFORM_SIDESLIP_ANGLE_FULL = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x4D};

void register_st0601(KLVRegistry& reg) {
    // Platform Heading Angle: degrees [0,360] -> uint16
    reg.register_ul(PLATFORM_HEADING_ANGLE, {
        [](double degrees) {
            uint16_t raw = static_cast<uint16_t>((degrees / 360.0) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return (raw / 65535.0) * 360.0;
        }
    });

    // Target Location Elevation: meters [-900,19000] -> uint16
    reg.register_ul(TARGET_LOCATION_ELEVATION, {
        [](double meters) {
            double min = -900.0, max = 19000.0;
            uint16_t raw = static_cast<uint16_t>(((meters - min) / (max - min)) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            double min = -900.0, max = 19000.0;
            return min + (raw / 65535.0) * (max - min);
        }
    });

    // Target Track Gate Width: pixels (step 2) -> uint8
    reg.register_ul(TARGET_TRACK_GATE_WIDTH, {
        [](double pixels) {
            uint8_t raw = static_cast<uint8_t>(pixels / 2.0);
            return std::vector<uint8_t>{raw};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return bytes[0] * 2.0;
        }
    });

    // Target Track Gate Height: pixels (step 2) -> uint8
    reg.register_ul(TARGET_TRACK_GATE_HEIGHT, {
        [](double pixels) {
            uint8_t raw = static_cast<uint8_t>(pixels / 2.0);
            return std::vector<uint8_t>{raw};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return bytes[0] * 2.0;
        }
    });

    // Target Track Gate Center X: pixels [0,65535] -> uint16
    reg.register_ul(TARGET_TRACK_GATE_X, {
        [](double pixels) {
            uint16_t raw = static_cast<uint16_t>(pixels);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return static_cast<double>(raw);
        }
    });

    // Target Track Gate Center Y: pixels [0,65535] -> uint16
    reg.register_ul(TARGET_TRACK_GATE_Y, {
        [](double pixels) {
            uint16_t raw = static_cast<uint16_t>(pixels);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return static_cast<double>(raw);
        }
    });

    // Sensor Latitude: degrees [-90,+90] -> int32
    reg.register_ul(SENSOR_LATITUDE, {
        [](double degrees) {
            double scale = 2147483647.0 / 90.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 90.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Sensor Longitude: degrees [-180,+180] -> int32
    reg.register_ul(SENSOR_LONGITUDE, {
        [](double degrees) {
            double scale = 2147483647.0 / 180.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 180.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Sensor Horizontal FOV: degrees [0,180] -> uint16
    reg.register_ul(SENSOR_HORIZONTAL_FOV, {
        [](double degrees) {
            uint16_t raw = static_cast<uint16_t>((degrees / 180.0) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return (raw / 65535.0) * 180.0;
        }
    });

    // Sensor Vertical FOV: degrees [0,180] -> uint16
    reg.register_ul(SENSOR_VERTICAL_FOV, {
        [](double degrees) {
            uint16_t raw = static_cast<uint16_t>((degrees / 180.0) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return (raw / 65535.0) * 180.0;
        }
    });

    // Frame Center Latitude: degrees [-90,+90] -> int32
    reg.register_ul(FRAME_CENTER_LATITUDE, {
        [](double degrees) {
            double scale = 2147483647.0 / 90.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 90.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Frame Center Longitude: degrees [-180,+180] -> int32
    reg.register_ul(FRAME_CENTER_LONGITUDE, {
        [](double degrees) {
            double scale = 2147483647.0 / 180.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 180.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Frame Center Elevation: meters [-900,19000] -> uint16
    reg.register_ul(FRAME_CENTER_ELEVATION, {
        [](double meters) {
            double min = -900.0, max = 19000.0;
            uint16_t raw = static_cast<uint16_t>(((meters - min) / (max - min)) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            double min = -900.0, max = 19000.0;
            return min + (raw / 65535.0) * (max - min);
        }
    });

    auto encode_offset = [](double deg) {
        int16_t raw = static_cast<int16_t>((deg / 0.15) * 65534.0);
        return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
    };
    auto decode_offset = [](const std::vector<uint8_t>& bytes) {
        if (bytes.size() != 2) return 0.0;
        int16_t raw = static_cast<int16_t>(bytes[0] << 8 | bytes[1]);
        return (raw / 65534.0) * 0.15;
    };

    reg.register_ul(OFFSET_CORNER_LAT_PT1, {encode_offset, decode_offset});
    reg.register_ul(OFFSET_CORNER_LON_PT1, {encode_offset, decode_offset});
    reg.register_ul(OFFSET_CORNER_LAT_PT2, {encode_offset, decode_offset});
    reg.register_ul(OFFSET_CORNER_LON_PT2, {encode_offset, decode_offset});
    reg.register_ul(OFFSET_CORNER_LAT_PT3, {encode_offset, decode_offset});
    reg.register_ul(OFFSET_CORNER_LON_PT3, {encode_offset, decode_offset});
    reg.register_ul(OFFSET_CORNER_LAT_PT4, {encode_offset, decode_offset});
    reg.register_ul(OFFSET_CORNER_LON_PT4, {encode_offset, decode_offset});

    // Simple coded values
    reg.register_ul(ICING_DETECTED, {
        [](double code) {
            return std::vector<uint8_t>{static_cast<uint8_t>(code)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return static_cast<double>(bytes[0]);
        }
    });

    // Wind direction: degrees [0,360]
    reg.register_ul(WIND_DIRECTION, {
        [](double deg) {
            uint16_t raw = static_cast<uint16_t>((deg / 360.0) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return (raw / 65535.0) * 360.0;
        }
    });

    // Wind speed: m/s [0,100]
    reg.register_ul(WIND_SPEED, {
        [](double speed) {
            uint8_t raw = static_cast<uint8_t>((speed / 100.0) * 255.0);
            return std::vector<uint8_t>{raw};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return (bytes[0] / 255.0) * 100.0;
        }
    });

    // Static pressure: millibar [0,5000]
    reg.register_ul(STATIC_PRESSURE, {
        [](double mb) {
            uint16_t raw = static_cast<uint16_t>((mb / 5000.0) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return (raw / 65535.0) * 5000.0;
        }
    });

    // Density altitude: meters [-900,19000]
    reg.register_ul(DENSITY_ALTITUDE, {
        [](double meters) {
            double min = -900.0, max = 19000.0;
            uint16_t raw = static_cast<uint16_t>(((meters - min) / (max - min)) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            double min = -900.0, max = 19000.0;
            return min + (raw / 65535.0) * (max - min);
        }
    });

    // Outside air temperature: int8 direct
    reg.register_ul(OUTSIDE_AIR_TEMPERATURE, {
        [](double c) {
            return std::vector<uint8_t>{static_cast<uint8_t>(static_cast<int8_t>(c))};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return static_cast<double>(static_cast<int8_t>(bytes[0]));
        }
    });

    // Target latitude/longitude: same scale as sensor
    reg.register_ul(TARGET_LATITUDE, {
        [](double degrees) {
            double scale = 2147483647.0 / 90.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 90.0 / 2147483647.0;
            return raw * scale;
        }
    });

    reg.register_ul(TARGET_LONGITUDE, {
        [](double degrees) {
            double scale = 2147483647.0 / 180.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 180.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Target error metrics: meters [0,4095]
    reg.register_ul(TARGET_ERROR_CE90, {
        [](double m) {
            uint16_t raw = static_cast<uint16_t>((m / 4095.0) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return (raw / 65535.0) * 4095.0;
        }
    });

    reg.register_ul(TARGET_ERROR_LE90, {
        [](double m) {
            uint16_t raw = static_cast<uint16_t>((m / 4095.0) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return (raw / 65535.0) * 4095.0;
        }
    });

    // Generic flag data
    reg.register_ul(GENERIC_FLAG_DATA01, {
        [](double val) {
            return std::vector<uint8_t>{static_cast<uint8_t>(val)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return static_cast<double>(bytes[0]);
        }
    });

    // Differential pressure: millibar [0,5000]
    reg.register_ul(DIFFERENTIAL_PRESSURE, {
        [](double mb) {
            uint16_t raw = static_cast<uint16_t>((mb / 5000.0) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return (raw / 65535.0) * 5000.0;
        }
    });

    // Platform angle of attack: degrees [-20,+20]
    auto encode_small_angle = [](double deg) {
        int16_t raw = static_cast<int16_t>((deg / 40.0) * 65534.0);
        return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
    };
    auto decode_small_angle = [](const std::vector<uint8_t>& bytes) {
        if (bytes.size() != 2) return 0.0;
        int16_t raw = static_cast<int16_t>(bytes[0] << 8 | bytes[1]);
        return (raw / 65534.0) * 40.0;
    };
    reg.register_ul(PLATFORM_ANGLE_OF_ATTACK, {encode_small_angle, decode_small_angle});
    reg.register_ul(PLATFORM_SIDESLIP_ANGLE, {encode_small_angle, decode_small_angle});

    // Platform vertical speed: m/s [-180,+180]
    reg.register_ul(PLATFORM_VERTICAL_SPEED, {
        [](double ms) {
            int16_t raw = static_cast<int16_t>((ms / 360.0) * 65534.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            int16_t raw = static_cast<int16_t>(bytes[0] << 8 | bytes[1]);
            return (raw / 65534.0) * 360.0;
        }
    });

    // Airfield barometric pressure: millibar [0,5000]
    reg.register_ul(AIRFIELD_BAROMETRIC_PRESSURE, {
        [](double mb) {
            uint16_t raw = static_cast<uint16_t>((mb / 5000.0) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return (raw / 65535.0) * 5000.0;
        }
    });

    // Airfield elevation: meters [-900,19000]
    reg.register_ul(AIRFIELD_ELEVATION, {
        [](double meters) {
            double min = -900.0, max = 19000.0;
            uint16_t raw = static_cast<uint16_t>(((meters - min) / (max - min)) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            double min = -900.0, max = 19000.0;
            return min + (raw / 65535.0) * (max - min);
        }
    });

    // Relative humidity: percent [0,100]
    reg.register_ul(RELATIVE_HUMIDITY, {
        [](double pct) {
            uint8_t raw = static_cast<uint8_t>((pct / 100.0) * 255.0);
            return std::vector<uint8_t>{raw};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return (bytes[0] / 255.0) * 100.0;
        }
    });

    // Platform ground speed: m/s [0,255]
    reg.register_ul(PLATFORM_GROUND_SPEED, {
        [](double speed) {
            return std::vector<uint8_t>{static_cast<uint8_t>(speed)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return static_cast<double>(bytes[0]);
        }
    });

    // Ground range: meters [0,5e6]
    reg.register_ul(GROUND_RANGE, {
        [](double meters) {
            uint32_t raw = static_cast<uint32_t>((meters / 5000000.0) * 4294967295.0);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            uint32_t raw = (static_cast<uint32_t>(bytes[0]) << 24) |
                           (static_cast<uint32_t>(bytes[1]) << 16) |
                           (static_cast<uint32_t>(bytes[2]) << 8) |
                           static_cast<uint32_t>(bytes[3]);
            return (raw / 4294967295.0) * 5000000.0;
        }
    });

    // Platform fuel remaining: kg [0,10000]
    reg.register_ul(PLATFORM_FUEL_REMAINING, {
        [](double kg) {
            uint16_t raw = static_cast<uint16_t>((kg / 10000.0) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return (raw / 65535.0) * 10000.0;
        }
    });

    // Weapon load: raw nibbles packed into uint16
    reg.register_ul(WEAPON_LOAD, {
        [](double val) {
            uint16_t raw = static_cast<uint16_t>(val);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return static_cast<double>(raw);
        }
    });

    // Weapon fired: station/substation nibbles
    reg.register_ul(WEAPON_FIRED, {
        [](double val) {
            return std::vector<uint8_t>{static_cast<uint8_t>(val)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return static_cast<double>(bytes[0]);
        }
    });

    // Laser PRF code: decimal in uint16
    reg.register_ul(LASER_PRF_CODE, {
        [](double val) {
            uint16_t raw = static_cast<uint16_t>(val);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return static_cast<double>(raw);
        }
    });

    // Sensor field of view name: enum
    reg.register_ul(SENSOR_FOV_NAME, {
        [](double val) {
            return std::vector<uint8_t>{static_cast<uint8_t>(val)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return static_cast<double>(bytes[0]);
        }
    });

    // Platform magnetic heading: degrees [0,360] -> uint16
    reg.register_ul(PLATFORM_MAGNETIC_HEADING, {
        [](double degrees) {
            uint16_t raw = static_cast<uint16_t>((degrees / 360.0) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return (raw / 65535.0) * 360.0;
        }
    });

    // UAS LS version number: uint8
    reg.register_ul(UAS_LS_VERSION_NUMBER, {
        [](double val) {
            return std::vector<uint8_t>{static_cast<uint8_t>(val)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return static_cast<double>(bytes[0]);
        }
    });

    // Alternate platform latitude: degrees [-90,+90] -> int32
    reg.register_ul(ALTERNATE_PLATFORM_LATITUDE, {
        [](double degrees) {
            double scale = 2147483647.0 / 90.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 90.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Alternate platform longitude: degrees [-180,+180] -> int32
    reg.register_ul(ALTERNATE_PLATFORM_LONGITUDE, {
        [](double degrees) {
            double scale = 2147483647.0 / 180.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 180.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Alternate platform altitude: meters [-900,19000] -> uint16
    reg.register_ul(ALTERNATE_PLATFORM_ALTITUDE, {
        [](double meters) {
            double min = -900.0, max = 19000.0;
            uint16_t raw = static_cast<uint16_t>(((meters - min) / (max - min)) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            double min = -900.0, max = 19000.0;
            return min + (raw / 65535.0) * (max - min);
        }
    });

    // Alternate platform heading: degrees [0,360] -> uint16
    reg.register_ul(ALTERNATE_PLATFORM_HEADING, {
        [](double degrees) {
            uint16_t raw = static_cast<uint16_t>((degrees / 360.0) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            return (raw / 65535.0) * 360.0;
        }
    });

    // Event start time UTC: microseconds since epoch -> uint64
    reg.register_ul(EVENT_START_TIME_UTC, {
        [](double usec) {
            uint64_t raw = static_cast<uint64_t>(usec);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 56),
                static_cast<uint8_t>(raw >> 48),
                static_cast<uint8_t>(raw >> 40),
                static_cast<uint8_t>(raw >> 32),
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 8) return 0.0;
            uint64_t raw =
                (static_cast<uint64_t>(bytes[0]) << 56) |
                (static_cast<uint64_t>(bytes[1]) << 48) |
                (static_cast<uint64_t>(bytes[2]) << 40) |
                (static_cast<uint64_t>(bytes[3]) << 32) |
                (static_cast<uint64_t>(bytes[4]) << 24) |
                (static_cast<uint64_t>(bytes[5]) << 16) |
                (static_cast<uint64_t>(bytes[6]) << 8)  |
                static_cast<uint64_t>(bytes[7]);
            return static_cast<double>(raw);
        }
    });

    // Sensor ellipsoid height: meters [-900,19000] -> uint16
    reg.register_ul(SENSOR_ELLIPSOID_HEIGHT, {
        [](double meters) {
            double min = -900.0, max = 19000.0;
            uint16_t raw = static_cast<uint16_t>(((meters - min) / (max - min)) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            double min = -900.0, max = 19000.0;
            return min + (raw / 65535.0) * (max - min);
        }
    });

    // Alternate platform ellipsoid height: meters [-900,19000] -> uint16
    reg.register_ul(ALTERNATE_PLATFORM_ELLIPSOID_HEIGHT, {
        [](double meters) {
            double min = -900.0, max = 19000.0;
            uint16_t raw = static_cast<uint16_t>(((meters - min) / (max - min)) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            double min = -900.0, max = 19000.0;
            return min + (raw / 65535.0) * (max - min);
        }
    });

    // Operational mode: enum
    reg.register_ul(OPERATIONAL_MODE, {
        [](double val) {
            return std::vector<uint8_t>{static_cast<uint8_t>(val)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return static_cast<double>(bytes[0]);
        }
    });

    // Frame center height above ellipsoid: meters [-900,19000]
    reg.register_ul(FRAME_CENTER_HAE, {
        [](double meters) {
            double min = -900.0, max = 19000.0;
            uint16_t raw = static_cast<uint16_t>(((meters - min) / (max - min)) * 65535.0);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = static_cast<uint16_t>(bytes[0] << 8 | bytes[1]);
            double min = -900.0, max = 19000.0;
            return min + (raw / 65535.0) * (max - min);
        }
    });

    // Sensor north velocity: m/s [-327,+327] -> int16
    reg.register_ul(SENSOR_NORTH_VELOCITY, {
        [](double ms) {
            double min = -327.0, max = 327.0;
            int16_t raw = static_cast<int16_t>(((ms - min) / (max - min)) * 65534.0 - 32767);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            int16_t raw = static_cast<int16_t>(bytes[0] << 8 | bytes[1]);
            double min = -327.0, max = 327.0;
            return min + ((raw + 32767.0) / 65534.0) * (max - min);
        }
    });

    // Sensor east velocity: m/s [-327,+327] -> int16
    reg.register_ul(SENSOR_EAST_VELOCITY, {
        [](double ms) {
            double min = -327.0, max = 327.0;
            int16_t raw = static_cast<int16_t>(((ms - min) / (max - min)) * 65534.0 - 32767);
            return std::vector<uint8_t>{static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw)};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            int16_t raw = static_cast<int16_t>(bytes[0] << 8 | bytes[1]);
            double min = -327.0, max = 327.0;
            return min + ((raw + 32767.0) / 65534.0) * (max - min);
        }
    });

    // Corner latitude point1 full: degrees [-90,+90] -> int32
    reg.register_ul(CORNER_LAT_PT1_FULL, {
        [](double degrees) {
            double scale = 2147483647.0 / 90.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 90.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Corner longitude point1 full: degrees [-180,+180] -> int32
    reg.register_ul(CORNER_LON_PT1_FULL, {
        [](double degrees) {
            double scale = 2147483647.0 / 180.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 180.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Corner latitude point2 full
    reg.register_ul(CORNER_LAT_PT2_FULL, {
        [](double degrees) {
            double scale = 2147483647.0 / 90.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 90.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Corner longitude point2 full
    reg.register_ul(CORNER_LON_PT2_FULL, {
        [](double degrees) {
            double scale = 2147483647.0 / 180.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 180.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Corner latitude point3 full
    reg.register_ul(CORNER_LAT_PT3_FULL, {
        [](double degrees) {
            double scale = 2147483647.0 / 90.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 90.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Corner longitude point3 full
    reg.register_ul(CORNER_LON_PT3_FULL, {
        [](double degrees) {
            double scale = 2147483647.0 / 180.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 180.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Corner latitude point4 full
    reg.register_ul(CORNER_LAT_PT4_FULL, {
        [](double degrees) {
            double scale = 2147483647.0 / 90.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 90.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Corner longitude point4 full
    reg.register_ul(CORNER_LON_PT4_FULL, {
        [](double degrees) {
            double scale = 2147483647.0 / 180.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 180.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Platform pitch angle full: degrees [-90,+90] -> int32
    reg.register_ul(PLATFORM_PITCH_ANGLE_FULL, {
        [](double degrees) {
            double scale = 2147483647.0 / 90.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 90.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Platform roll angle full
    reg.register_ul(PLATFORM_ROLL_ANGLE_FULL, {
        [](double degrees) {
            double scale = 2147483647.0 / 90.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 90.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Platform angle of attack full
    reg.register_ul(PLATFORM_AOA_FULL, {
        [](double degrees) {
            double scale = 2147483647.0 / 90.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 90.0 / 2147483647.0;
            return raw * scale;
        }
    });

    // Platform sideslip angle full
    reg.register_ul(PLATFORM_SIDESLIP_ANGLE_FULL, {
        [](double degrees) {
            double scale = 2147483647.0 / 90.0;
            int32_t raw = static_cast<int32_t>(degrees * scale);
            return std::vector<uint8_t>{
                static_cast<uint8_t>(raw >> 24),
                static_cast<uint8_t>(raw >> 16),
                static_cast<uint8_t>(raw >> 8),
                static_cast<uint8_t>(raw)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 4) return 0.0;
            int32_t raw = static_cast<int32_t>(
                (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
            double scale = 90.0 / 2147483647.0;
            return raw * scale;
        }
    });
}

} // namespace st0601
} // namespace misb
