#pragma once

#include "klv.h"

namespace misb {
namespace st0601 {

// Universal Labels for a subset of MISB ST0601 tags
extern const UL PLATFORM_HEADING_ANGLE;
extern const UL TARGET_LOCATION_ELEVATION;
extern const UL TARGET_TRACK_GATE_WIDTH;
extern const UL TARGET_TRACK_GATE_HEIGHT;
extern const UL TARGET_TRACK_GATE_X;
extern const UL TARGET_TRACK_GATE_Y;
extern const UL SENSOR_LATITUDE;
extern const UL SENSOR_LONGITUDE;
extern const UL SENSOR_HORIZONTAL_FOV;
extern const UL SENSOR_VERTICAL_FOV;
extern const UL FRAME_CENTER_LATITUDE;
extern const UL FRAME_CENTER_LONGITUDE;
extern const UL FRAME_CENTER_ELEVATION;
extern const UL OFFSET_CORNER_LAT_PT1;
extern const UL OFFSET_CORNER_LON_PT1;
extern const UL OFFSET_CORNER_LAT_PT2;
extern const UL OFFSET_CORNER_LON_PT2;
extern const UL OFFSET_CORNER_LAT_PT3;
extern const UL OFFSET_CORNER_LON_PT3;
extern const UL OFFSET_CORNER_LAT_PT4;
extern const UL OFFSET_CORNER_LON_PT4;
extern const UL ICING_DETECTED;
extern const UL WIND_DIRECTION;
extern const UL WIND_SPEED;
extern const UL STATIC_PRESSURE;
extern const UL DENSITY_ALTITUDE;
extern const UL OUTSIDE_AIR_TEMPERATURE;
extern const UL TARGET_LATITUDE;
extern const UL TARGET_LONGITUDE;
extern const UL TARGET_ERROR_CE90;
extern const UL TARGET_ERROR_LE90;
extern const UL GENERIC_FLAG_DATA01;
extern const UL DIFFERENTIAL_PRESSURE;
extern const UL PLATFORM_ANGLE_OF_ATTACK;
extern const UL PLATFORM_VERTICAL_SPEED;
extern const UL PLATFORM_SIDESLIP_ANGLE;
extern const UL AIRFIELD_BAROMETRIC_PRESSURE;
extern const UL AIRFIELD_ELEVATION;
extern const UL RELATIVE_HUMIDITY;
extern const UL PLATFORM_GROUND_SPEED;
extern const UL GROUND_RANGE;
extern const UL PLATFORM_FUEL_REMAINING;
extern const UL WEAPON_LOAD;
extern const UL WEAPON_FIRED;
extern const UL LASER_PRF_CODE;
extern const UL SENSOR_FOV_NAME;
extern const UL PLATFORM_MAGNETIC_HEADING;
extern const UL UAS_LS_VERSION_NUMBER;
extern const UL ALTERNATE_PLATFORM_LATITUDE;
extern const UL ALTERNATE_PLATFORM_LONGITUDE;
extern const UL ALTERNATE_PLATFORM_ALTITUDE;
extern const UL ALTERNATE_PLATFORM_HEADING;
extern const UL EVENT_START_TIME_UTC;
extern const UL SENSOR_ELLIPSOID_HEIGHT;
extern const UL ALTERNATE_PLATFORM_ELLIPSOID_HEIGHT;
extern const UL OPERATIONAL_MODE;
extern const UL FRAME_CENTER_HAE;
extern const UL SENSOR_NORTH_VELOCITY;
extern const UL SENSOR_EAST_VELOCITY;
extern const UL CORNER_LAT_PT1_FULL;
extern const UL CORNER_LON_PT1_FULL;
extern const UL CORNER_LAT_PT2_FULL;
extern const UL CORNER_LON_PT2_FULL;
extern const UL CORNER_LAT_PT3_FULL;
extern const UL CORNER_LON_PT3_FULL;
extern const UL CORNER_LAT_PT4_FULL;
extern const UL CORNER_LON_PT4_FULL;
extern const UL PLATFORM_PITCH_ANGLE_FULL;
extern const UL PLATFORM_ROLL_ANGLE_FULL;
extern const UL PLATFORM_AOA_FULL;
extern const UL PLATFORM_SIDESLIP_ANGLE_FULL;

// Register encode/decode lambdas for the above ULs
void register_st0601(KLVRegistry& reg);

} // namespace st0601
} // namespace misb
