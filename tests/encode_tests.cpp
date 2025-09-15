#include "klv.h"
#include "klv_macros.h"
#include "st0601.h"
#include "st0102.h"
#include "st0903.h"
#include "st_common.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <memory>
#include <vector>

int main() {
    auto& reg = KLVRegistry::instance();
    misb::st0601::register_st0601(reg);
    misb::st0102::register_st0102(reg);
    misb::st0903::register_st0903(reg);

    // ST0601 encoding
    KLVSet st0601_set(false, misb::st0601::ST_ID);
    st0601_set.add(std::make_shared<KLVLeaf>(misb::st0601::PLATFORM_HEADING_ANGLE, 90.0, true));
    st0601_set.add(std::make_shared<KLVLeaf>(misb::st0601::SENSOR_LATITUDE, 45.0, true));
    st0601_set.add(std::make_shared<KLVLeaf>(misb::st0601::SENSOR_LONGITUDE, -75.0, true));
    std::vector<uint8_t> expected0601 = {
        0x05,0x02,0x40,0x00,
        0x0D,0x04,0x40,0x00,0x00,0x00,
        0x0E,0x04,0xCA,0xAA,0xAA,0xAB
    };
    auto encoded0601 = st0601_set.encode();
    assert(encoded0601 == expected0601);
    KLVSet decoded0601(false, misb::st0601::ST_ID);
    decoded0601.decode(encoded0601);
    double heading=0, lat=0, lon=0;
    for (const auto& node : decoded0601.children()) {
        if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node)) {
            if (leaf->ul() == misb::st0601::PLATFORM_HEADING_ANGLE) heading = leaf->value();
            else if (leaf->ul() == misb::st0601::SENSOR_LATITUDE) lat = leaf->value();
            else if (leaf->ul() == misb::st0601::SENSOR_LONGITUDE) lon = leaf->value();
        }
    }
    assert(std::fabs(heading - 90.0) < 1e-2);
    assert(std::fabs(lat - 45.0) < 1e-6);
    assert(std::fabs(lon - (-75.0)) < 1e-6);

    // ST0102 encoding
    KLVSet st0102_set(false, misb::st0102::ST_ID);
    st0102_set.add(std::make_shared<KLVLeaf>(misb::st0102::CLASSIFICATION, 2.0, true));
    st0102_set.add(std::make_shared<KLVLeaf>(misb::st0102::CLASSIFICATION_SYSTEM, 1.0, true));
    std::vector<uint8_t> expected0102 = {0x00,0x01,0x02, 0x01,0x01,0x01};
    auto encoded0102 = st0102_set.encode();
    assert(encoded0102 == expected0102);
    KLVSet decoded0102(false, misb::st0102::ST_ID);
    decoded0102.decode(encoded0102);
    double cls=0, clsSys=0;
    for (const auto& node : decoded0102.children()) {
        if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node)) {
            if (leaf->ul() == misb::st0102::CLASSIFICATION) cls = leaf->value();
            else if (leaf->ul() == misb::st0102::CLASSIFICATION_SYSTEM) clsSys = leaf->value();
        }
    }
    assert(cls == 2.0);
    assert(clsSys == 1.0);

    // ST0903 encoding
    KLVSet st0903_set(false, misb::st0903::ST_ID);
    st0903_set.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_TARGET_ID, 42.0, true));
    st0903_set.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_DETECTION_PROBABILITY, 0.5, true));
    std::vector<uint8_t> expected0903 = {
        0x00,0x02,0x00,0x2A,
        0x02,0x01,0x7F
    };
    auto encoded0903 = st0903_set.encode();
    assert(encoded0903 == expected0903);
    KLVSet decoded0903(false, misb::st0903::ST_ID);
    decoded0903.decode(encoded0903);
    double id=0, prob=0;
    for (const auto& node : decoded0903.children()) {
        if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node)) {
            if (leaf->ul() == misb::st0903::VMTI_TARGET_ID) id = leaf->value();
            else if (leaf->ul() == misb::st0903::VMTI_DETECTION_PROBABILITY) prob = leaf->value();
        }
    }
    assert(id == 42.0);
    assert(std::fabs(prob - 0.5) < 1e-2);

    // STANAG 4609 packet assembly
    auto packet = STANAG4609_PACKET(
        KLV_ST_ITEM(0601, UNIX_TIMESTAMP, 1700000000.0),
        KLV_ST_ITEM(0601, SENSOR_LATITUDE, 48.0),
        KLV_ST_ITEM(0601, SENSOR_LONGITUDE, 2.0)
    );
    // Verify outer UL
    assert(packet.size() > 17);
    std::vector<uint8_t> expected_ul = {
        0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,
        0x0E,0x01,0x03,0x01,0x01,0x00,0x00,0x00
    };
    assert(std::equal(expected_ul.begin(), expected_ul.end(), packet.begin()));
    size_t payload_len = 0, len_bytes = 0;
    bool ok = misb::decode_ber_length(packet, 16, payload_len, len_bytes);
    assert(ok);
    assert(payload_len == packet.size() - 16 - len_bytes);
    std::vector<uint8_t> payload(packet.begin() + 16 + len_bytes, packet.end());
    // Verify checksum over the entire packet
    uint16_t crc_stored = (static_cast<uint16_t>(packet[packet.size() - 2]) << 8) |
                          packet.back();
    uint16_t crc_calc = misb::klv_checksum_16(std::vector<uint8_t>(packet.begin(), packet.end() - 2));
    assert(crc_stored == crc_calc);
    // Remove checksum TLV from payload before decoding
    payload.resize(payload.size() - 4);
    KLVSet decoded(false, misb::st0601::ST_ID);
    decoded.decode(payload);
    double ts = 0.0, flat = 0.0, flon = 0.0, ver = 0.0;
    ST_GET(decoded, 0601, UNIX_TIMESTAMP, ts);
    ST_GET(decoded, 0601, SENSOR_LATITUDE, flat);
    ST_GET(decoded, 0601, SENSOR_LONGITUDE, flon);
    ST_GET(decoded, 0601, UAS_LS_VERSION_NUMBER, ver);
    assert(std::fabs(ts - 1700000000.0) < 1e-3);
    assert(std::fabs(flat - 48.0) < 1e-6);
    assert(std::fabs(flon - 2.0) < 1e-6);
    assert(std::fabs(ver - 12.0) < 1e-6);

    return 0;
}
