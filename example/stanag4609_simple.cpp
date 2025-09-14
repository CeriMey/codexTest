#include "klv_macros.h"
#include "st0601.h"
#include <iostream>
#include <iomanip>

int main() {
    auto& reg = KLVRegistry::instance();
    misb::st0601::register_st0601(reg);

    // Build a simple STANAG 4609 packet using nested macros
    KLVSet packet = STANAG4609_PACKET(
        KLV_ST_ITEM(0601, UNIX_TIMESTAMP, 1700000000.0),
        KLV_ST_ITEM(0601, SENSOR_LATITUDE, 48.8566),
        KLV_ST_ITEM(0601, SENSOR_LONGITUDE, 2.3522),
        KLV_ST_ITEM(0601, PLATFORM_HEADING_ANGLE, 90.0));
    auto bytes = packet.encode();

    std::cout << "Encoded packet:";
    for (uint8_t b : bytes) {
        std::cout << ' ' << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(b);
    }
    std::cout << std::dec << "\n";

    // Decode the packet
    KLVSet decoded;
    KLV_DECODE_SET(decoded, bytes);

    double ts = 0.0, lat = 0.0, lon = 0.0, heading = 0.0;
    ST_GET(decoded, 0601, UNIX_TIMESTAMP, ts);
    ST_GET(decoded, 0601, SENSOR_LATITUDE, lat);
    ST_GET(decoded, 0601, SENSOR_LONGITUDE, lon);
    ST_GET(decoded, 0601, PLATFORM_HEADING_ANGLE, heading);

    std::cout << "Decoded values:\n";
    std::cout << "  Timestamp: " << ts << "\n";
    std::cout << "  Latitude : " << lat << "\n";
    std::cout << "  Longitude: " << lon << "\n";
    std::cout << "  Heading  : " << heading << "\n";
    return 0;
}
