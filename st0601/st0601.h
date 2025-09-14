#pragma once

#include "klv.h"
#include "st_common.h"

namespace misb {
namespace st0601 {

constexpr uint8_t ST_ID = 0x01;
constexpr UL PLATFORM_HEADING_ANGLE = make_st_ul(ST_ID, 0x00);
constexpr UL UNIX_TIMESTAMP = make_st_ul(ST_ID, 0x01);
constexpr UL TARGET_LOCATION_ELEVATION = make_st_ul(ST_ID, 0x01);
constexpr UL TARGET_TRACK_GATE_WIDTH = make_st_ul(ST_ID, 0x02);
constexpr UL TARGET_TRACK_GATE_HEIGHT = make_st_ul(ST_ID, 0x03);
constexpr UL TARGET_TRACK_GATE_X = make_st_ul(ST_ID, 0x04);
constexpr UL TARGET_TRACK_GATE_Y = make_st_ul(ST_ID, 0x05);
constexpr UL SENSOR_LATITUDE = make_st_ul(ST_ID, 0x06);
constexpr UL SENSOR_LONGITUDE = make_st_ul(ST_ID, 0x07);
constexpr UL SENSOR_HORIZONTAL_FOV = make_st_ul(ST_ID, 0x08);
constexpr UL SENSOR_VERTICAL_FOV = make_st_ul(ST_ID, 0x09);
constexpr UL FRAME_CENTER_LATITUDE = make_st_ul(ST_ID, 0x0A);
constexpr UL FRAME_CENTER_LONGITUDE = make_st_ul(ST_ID, 0x0B);
constexpr UL FRAME_CENTER_ELEVATION = make_st_ul(ST_ID, 0x0C);
constexpr UL OFFSET_CORNER_LAT_PT1 = make_st_ul(ST_ID, 0x0D);
constexpr UL OFFSET_CORNER_LON_PT1 = make_st_ul(ST_ID, 0x0E);
constexpr UL OFFSET_CORNER_LAT_PT2 = make_st_ul(ST_ID, 0x0F);
constexpr UL OFFSET_CORNER_LON_PT2 = make_st_ul(ST_ID, 0x10);
constexpr UL OFFSET_CORNER_LAT_PT3 = make_st_ul(ST_ID, 0x11);
constexpr UL OFFSET_CORNER_LON_PT3 = make_st_ul(ST_ID, 0x12);
constexpr UL OFFSET_CORNER_LAT_PT4 = make_st_ul(ST_ID, 0x13);
constexpr UL OFFSET_CORNER_LON_PT4 = make_st_ul(ST_ID, 0x14);
constexpr UL ICING_DETECTED = make_st_ul(ST_ID, 0x15);
constexpr UL WIND_DIRECTION = make_st_ul(ST_ID, 0x16);
constexpr UL WIND_SPEED = make_st_ul(ST_ID, 0x17);
constexpr UL STATIC_PRESSURE = make_st_ul(ST_ID, 0x18);
constexpr UL DENSITY_ALTITUDE = make_st_ul(ST_ID, 0x19);
constexpr UL OUTSIDE_AIR_TEMPERATURE = make_st_ul(ST_ID, 0x1A);
constexpr UL TARGET_LATITUDE = make_st_ul(ST_ID, 0x1B);
constexpr UL TARGET_LONGITUDE = make_st_ul(ST_ID, 0x1C);
constexpr UL TARGET_ERROR_CE90 = make_st_ul(ST_ID, 0x1E);
constexpr UL TARGET_ERROR_LE90 = make_st_ul(ST_ID, 0x1F);
constexpr UL GENERIC_FLAG_DATA01 = make_st_ul(ST_ID, 0x20);
constexpr UL DIFFERENTIAL_PRESSURE = make_st_ul(ST_ID, 0x22);
constexpr UL PLATFORM_ANGLE_OF_ATTACK = make_st_ul(ST_ID, 0x23);
constexpr UL PLATFORM_VERTICAL_SPEED = make_st_ul(ST_ID, 0x24);
constexpr UL PLATFORM_SIDESLIP_ANGLE = make_st_ul(ST_ID, 0x25);
constexpr UL AIRFIELD_BAROMETRIC_PRESSURE = make_st_ul(ST_ID, 0x26);
constexpr UL AIRFIELD_ELEVATION = make_st_ul(ST_ID, 0x27);
constexpr UL RELATIVE_HUMIDITY = make_st_ul(ST_ID, 0x28);
constexpr UL PLATFORM_GROUND_SPEED = make_st_ul(ST_ID, 0x29);
constexpr UL GROUND_RANGE = make_st_ul(ST_ID, 0x2A);
constexpr UL PLATFORM_FUEL_REMAINING = make_st_ul(ST_ID, 0x2B);
constexpr UL WEAPON_LOAD = make_st_ul(ST_ID, 0x2C);
constexpr UL WEAPON_FIRED = make_st_ul(ST_ID, 0x2D);
constexpr UL LASER_PRF_CODE = make_st_ul(ST_ID, 0x2E);
constexpr UL SENSOR_FOV_NAME = make_st_ul(ST_ID, 0x2F);
constexpr UL PLATFORM_MAGNETIC_HEADING = make_st_ul(ST_ID, 0x30);
constexpr UL UAS_LS_VERSION_NUMBER = make_st_ul(ST_ID, 0x31);
constexpr UL ALTERNATE_PLATFORM_LATITUDE = make_st_ul(ST_ID, 0x33);
constexpr UL ALTERNATE_PLATFORM_LONGITUDE = make_st_ul(ST_ID, 0x34);
constexpr UL ALTERNATE_PLATFORM_ALTITUDE = make_st_ul(ST_ID, 0x35);
constexpr UL ALTERNATE_PLATFORM_HEADING = make_st_ul(ST_ID, 0x37);
constexpr UL EVENT_START_TIME_UTC = make_st_ul(ST_ID, 0x38);
constexpr UL SENSOR_ELLIPSOID_HEIGHT = make_st_ul(ST_ID, 0x3B);
constexpr UL ALTERNATE_PLATFORM_ELLIPSOID_HEIGHT = make_st_ul(ST_ID, 0x3C);
constexpr UL OPERATIONAL_MODE = make_st_ul(ST_ID, 0x3D);
constexpr UL FRAME_CENTER_HAE = make_st_ul(ST_ID, 0x3E);
constexpr UL SENSOR_NORTH_VELOCITY = make_st_ul(ST_ID, 0x3F);
constexpr UL SENSOR_EAST_VELOCITY = make_st_ul(ST_ID, 0x40);
constexpr UL CORNER_LAT_PT1_FULL = make_st_ul(ST_ID, 0x42);
constexpr UL CORNER_LON_PT1_FULL = make_st_ul(ST_ID, 0x43);
constexpr UL CORNER_LAT_PT2_FULL = make_st_ul(ST_ID, 0x44);
constexpr UL CORNER_LON_PT2_FULL = make_st_ul(ST_ID, 0x45);
constexpr UL CORNER_LAT_PT3_FULL = make_st_ul(ST_ID, 0x46);
constexpr UL CORNER_LON_PT3_FULL = make_st_ul(ST_ID, 0x47);
constexpr UL CORNER_LAT_PT4_FULL = make_st_ul(ST_ID, 0x48);
constexpr UL CORNER_LON_PT4_FULL = make_st_ul(ST_ID, 0x49);
constexpr UL VMTI_LOCAL_SET = make_st_ul(ST_ID, 0x4A);
constexpr UL PLATFORM_PITCH_ANGLE_FULL = make_st_ul(ST_ID, 0x4B);
constexpr UL PLATFORM_ROLL_ANGLE_FULL = make_st_ul(ST_ID, 0x4C);
constexpr UL PLATFORM_AOA_FULL = make_st_ul(ST_ID, 0x4D);
constexpr UL PLATFORM_SIDESLIP_ANGLE_FULL = make_st_ul(ST_ID, 0x4E);
constexpr UL MIIS_CORE_IDENTIFIER = make_st_ul(ST_ID, 0x4F);
constexpr UL SAR_MOTION_IMAGERY_LOCAL_SET = make_st_ul(ST_ID, 0x50);
constexpr UL TARGET_WIDTH_EXTENDED = make_st_ul(ST_ID, 0x51);
constexpr UL RANGE_IMAGE_LOCAL_SET = make_st_ul(ST_ID, 0x52);
constexpr UL GEO_REGISTRATION_LOCAL_SET = make_st_ul(ST_ID, 0x53);
constexpr UL COMPOSITE_IMAGING_LOCAL_SET = make_st_ul(ST_ID, 0x54);
constexpr UL SEGMENT_LOCAL_SET = make_st_ul(ST_ID, 0x55);
constexpr UL AMEND_LOCAL_SET = make_st_ul(ST_ID, 0x56);
constexpr UL SDCC_FLP = make_st_ul(ST_ID, 0x57);
constexpr UL DENSITY_ALTITUDE_EXTENDED = make_st_ul(ST_ID, 0x58);
constexpr UL SENSOR_ELLIPSOID_HEIGHT_EXTENDED = make_st_ul(ST_ID, 0x59);
constexpr UL ALTERNATE_PLATFORM_ELLIPSOID_HEIGHT_EXTENDED = make_st_ul(ST_ID, 0x5A);
constexpr UL STREAM_DESIGNATOR = make_st_ul(ST_ID, 0x5B);
constexpr UL OPERATIONAL_BASE = make_st_ul(ST_ID, 0x5C);
constexpr UL BROADCAST_SOURCE = make_st_ul(ST_ID, 0x5D);
constexpr UL RANGE_TO_RECOVERY_LOCATION = make_st_ul(ST_ID, 0x5E);
constexpr UL TIME_AIRBORNE = make_st_ul(ST_ID, 0x5F);
constexpr UL PROPULSION_UNIT_SPEED = make_st_ul(ST_ID, 0x60);
constexpr UL PLATFORM_COURSE_ANGLE = make_st_ul(ST_ID, 0x61);
constexpr UL ALTITUDE_AGL = make_st_ul(ST_ID, 0x62);
constexpr UL RADAR_ALTIMETER = make_st_ul(ST_ID, 0x63);
constexpr UL CONTROL_COMMAND = make_st_ul(ST_ID, 0x64);
constexpr UL CONTROL_COMMAND_VERIFICATION_LIST = make_st_ul(ST_ID, 0x65);
constexpr UL SENSOR_AZIMUTH_RATE = make_st_ul(ST_ID, 0x66);
constexpr UL SENSOR_ELEVATION_RATE = make_st_ul(ST_ID, 0x67);
constexpr UL SENSOR_ROLL_RATE = make_st_ul(ST_ID, 0x68);
constexpr UL ON_BOARD_MI_STORAGE_PERCENT_FULL = make_st_ul(ST_ID, 0x69);
constexpr UL ACTIVE_WAVELENGTH_LIST = make_st_ul(ST_ID, 0x6A);
constexpr UL COUNTRY_CODES = make_st_ul(ST_ID, 0x6B);
constexpr UL NUMBER_OF_NAVSATS_IN_VIEW = make_st_ul(ST_ID, 0x6C);
constexpr UL POSITIONING_METHOD_SOURCE = make_st_ul(ST_ID, 0x6D);
constexpr UL PLATFORM_STATUS = make_st_ul(ST_ID, 0x6E);
constexpr UL SENSOR_CONTROL_MODE = make_st_ul(ST_ID, 0x6F);
constexpr UL SENSOR_FRAME_RATE_PACK = make_st_ul(ST_ID, 0x70);
constexpr UL WAVELENGTHS_LIST = make_st_ul(ST_ID, 0x71);
constexpr UL TARGET_ID = make_st_ul(ST_ID, 0x72);
constexpr UL AIRBASE_LOCATIONS = make_st_ul(ST_ID, 0x73);
constexpr UL TAKE_OFF_TIME = make_st_ul(ST_ID, 0x74);
constexpr UL TRANSMISSION_FREQUENCY = make_st_ul(ST_ID, 0x75);
constexpr UL ON_BOARD_MI_STORAGE_CAPACITY = make_st_ul(ST_ID, 0x76);
constexpr UL ZOOM_PERCENTAGE = make_st_ul(ST_ID, 0x77);
constexpr UL COMMUNICATIONS_METHOD = make_st_ul(ST_ID, 0x78);
constexpr UL LEAP_SECONDS = make_st_ul(ST_ID, 0x79);
constexpr UL CORRECTION_OFFSET = make_st_ul(ST_ID, 0x7A);
constexpr UL PAYLOAD_LIST = make_st_ul(ST_ID, 0x7B);
constexpr UL ACTIVE_PAYLOADS = make_st_ul(ST_ID, 0x7C);
constexpr UL WEAPONS_STORES = make_st_ul(ST_ID, 0x7D);
constexpr UL WAYPOINT_LIST = make_st_ul(ST_ID, 0x7E);

// Register encode/decode lambdas for the above ULs
void register_st0601(KLVRegistry& reg);

} // namespace st0601
} // namespace misb
