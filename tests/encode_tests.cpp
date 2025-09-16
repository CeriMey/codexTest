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
#include <string>
#include <stdexcept>
#include <limits>

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

    // ST0903 encoding (top-level metadata and vTarget series)
    std::vector<stanag::TagValue> vtarget_entries = {
        {misb::st0903::VTARGET_CENTROID, 1234.0},
        {misb::st0903::VTARGET_CENTROID_ROW, 25.0},
        {misb::st0903::VTARGET_CENTROID_COLUMN, 50.0},
        {misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.5},
        {misb::st0903::VTARGET_DETECTION_STATUS, 1.0}
    };
    misb::st0903::VTargetPack pack{42u, stanag::create_dataset(vtarget_entries, false)};
    auto vtarget_series = misb::st0903::encode_vtarget_series({pack});

    bool duplicate_error = false;
    try {
        misb::st0903::encode_vtarget_series({pack, pack});
    } catch (const std::runtime_error&) {
        duplicate_error = true;
    }
    assert(duplicate_error);

    KLVSet st0903_set(false, misb::st0903::ST_ID);
    st0903_set.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_FRAME_WIDTH, 1920.0, true));
    st0903_set.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_FRAME_HEIGHT, 1080.0, true));
    st0903_set.add(std::make_shared<KLVBytes>(misb::st0903::VMTI_VTARGET_SERIES, vtarget_series, true));

    auto encoded0903 = st0903_set.encode();
    KLVSet decoded0903(false, misb::st0903::ST_ID);
    decoded0903.decode(encoded0903);
    double width = 0.0, height = 0.0;
    for (const auto& node : decoded0903.children()) {
        if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node)) {
            if (leaf->ul() == misb::st0903::VMTI_FRAME_WIDTH) width = leaf->value();
            else if (leaf->ul() == misb::st0903::VMTI_FRAME_HEIGHT) height = leaf->value();
        } else if (auto bytes = std::dynamic_pointer_cast<KLVBytes>(node)) {
            if (bytes->ul() == misb::st0903::VMTI_VTARGET_SERIES) {
                auto decoded_packs = misb::st0903::decode_vtarget_series(bytes->value());
                assert(decoded_packs.size() == 1);
                assert(decoded_packs.front().target_id == 42u);
                const auto& vset = decoded_packs.front().set;
                double centroid = 0.0;
                double confidence = 0.0;
                for (const auto& child : vset.children()) {
                    if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(child)) {
                        if (leaf->ul() == misb::st0903::VTARGET_CENTROID) centroid = leaf->value();
                        else if (leaf->ul() == misb::st0903::VTARGET_CONFIDENCE_LEVEL) confidence = leaf->value();
                    }
                }
                assert(std::fabs(centroid - 1234.0) < 1e-6);
                assert(std::fabs(confidence - 0.5) < 1e-6);
            }
        }
    }
    assert(width == 1920.0);
    assert(height == 1080.0);

    auto ascii = [](const std::string& s) {
        return std::vector<uint8_t>(s.begin(), s.end());
    };

    auto find_value = [](const KLVSet& set, const UL& ul) {
        for (const auto& node : set.children()) {
            if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node)) {
                if (leaf->ul() == ul) {
                    return leaf->value();
                }
            }
        }
        return std::numeric_limits<double>::quiet_NaN();
    };

    auto algorithm_series = KLV_ALGORITHM_SERIES(
        KLV_ALGORITHM_SET(
            KLV_LOCAL_LEAF(misb::st0903::ALGORITHM_ID, 7.0),
            KLV_LOCAL_BYTES(misb::st0903::ALGORITHM_NAME, ascii("Detector")),
            KLV_LOCAL_LEAF(misb::st0903::ALGORITHM_CONFIDENCE, 0.75)
        ),
        KLV_ALGORITHM_SET(
            KLV_LOCAL_LEAF(misb::st0903::ALGORITHM_ID, 8.0),
            KLV_LOCAL_BYTES(misb::st0903::ALGORITHM_NAME, ascii("Tracker")),
            KLV_LOCAL_LEAF(misb::st0903::ALGORITHM_CONFIDENCE, 0.6)
        )
    );

    auto ontology_series = KLV_ONTOLOGY_SERIES(
        KLV_ONTOLOGY_SET(
            KLV_LOCAL_LEAF(misb::st0903::ONTOLOGY_ID, 301.0),
            KLV_LOCAL_BYTES(misb::st0903::ONTOLOGY_URI, ascii("urn:test:vehicle")),
            KLV_LOCAL_LEAF(misb::st0903::ONTOLOGY_CONFIDENCE, 0.8)
        )
    );

    auto decoded_algorithms = misb::st0903::decode_algorithm_series(algorithm_series);
    assert(decoded_algorithms.size() == 2);
    double alg0_id = find_value(decoded_algorithms[0], misb::st0903::ALGORITHM_ID);
    double alg1_id = find_value(decoded_algorithms[1], misb::st0903::ALGORITHM_ID);
    assert(std::fabs(alg0_id - 7.0) < 1e-6);
    assert(std::fabs(alg1_id - 8.0) < 1e-6);

    auto decoded_ontologies = misb::st0903::decode_ontology_series(ontology_series);
    assert(decoded_ontologies.size() == 1);
    double ontology_id = find_value(decoded_ontologies.front(), misb::st0903::ONTOLOGY_ID);
    assert(std::fabs(ontology_id - 301.0) < 1e-6);

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

    // Test BER long-form length for tag-based items
    std::vector<uint8_t> big_vec(130, 0xAB);
    UL big_ul = misb::make_st_ul(misb::st0601::ST_ID, 0x7F);
    KLVBytes big_bytes(big_ul, big_vec, true);
    auto encoded_big = big_bytes.encode();
    assert(encoded_big[0] == 0x7F);
    assert(encoded_big[1] == 0x81 && encoded_big[2] == 0x82);
    assert(encoded_big.size() == 1 + 2 + big_vec.size());
    KLVBytes big_bytes_dec(big_ul, {}, true);
    big_bytes_dec.decode(encoded_big);
    assert(big_bytes_dec.value() == big_vec);

    return 0;
}
