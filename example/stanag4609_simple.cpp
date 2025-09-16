#include "klv_macros.h"
#include "st0601.h"
#include "st0903.h"
#include "st_common.h"
#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <limits>

static double pixel_index(double row, double column, double frameWidth) {
    return column + ((row - 1.0) * frameWidth);
}

static double get_value(const KLVSet& set, const UL& ul) {
    for (const auto& node : set.children()) {
        if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node)) {
            if (leaf->ul() == ul) return leaf->value();
        }
    }
    return std::numeric_limits<double>::quiet_NaN();
}

int main() {
    auto& reg = KLVRegistry::instance();
    misb::st0601::register_st0601(reg);
    misb::st0903::register_st0903(reg);

    const double frameWidth = 1920.0;

    // Build a VMTI local set with 20 detections
    std::vector<misb::st0903::VTargetPack> packs;
    packs.reserve(20);
    for (int i = 1; i <= 20; ++i) {
        double row = 100.0 + i * 5.0;
        double col = 200.0 + i * 3.0;
        double confidence = 0.4 + 0.01 * static_cast<double>(i);
        double status = (i % 2 == 0) ? 1.0 : 0.0;
        packs.push_back(KLV_VTARGET_PACK(
            i,
            KLV_TAG(misb::st0903::VTARGET_CENTROID, pixel_index(row, col, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, row),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, col),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, confidence),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, status)
        ));
    }

    auto series = misb::st0903::encode_vtarget_series(packs);

    const double detectionCount = 20.0;
    KLVSet vmti = KLV_LOCAL_DATASET(
        KLV_TAG(misb::st0903::VMTI_LS_VERSION, 6.0),
        KLV_TAG(misb::st0903::VMTI_TOTAL_TARGETS_DETECTED, detectionCount),
        KLV_TAG(misb::st0903::VMTI_NUM_TARGETS_REPORTED, detectionCount),
        KLV_TAG(misb::st0903::VMTI_FRAME_WIDTH, frameWidth)
    );
    KLV_ADD_BYTES(vmti, misb::st0903::VMTI_VTARGET_SERIES, series);

    // Compose the STANAG 4609 packet (UAS LS version tag added automatically)
    auto packet = STANAG4609_PACKET(
        KLV_ST_ITEM(0601, UNIX_TIMESTAMP, 1700000000.0),
        KLV_ST_ITEM(0601, SENSOR_LATITUDE, 48.8566),
        KLV_ST_ITEM(0601, SENSOR_LONGITUDE, 2.3522),
        KLV_TAG(misb::st0601::VMTI_LOCAL_SET, vmti)
    );

    std::cout << "Encoded packet:";
    for (uint8_t b : packet) {
        std::cout << ' ' << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(b);
    }
    std::cout << std::dec << '\n';

    // Decode the packet: skip the 16-byte UL and BER length
    if (packet.size() <= 16) return 0;
    size_t payload_len = 0, len_bytes = 0;
    if (!misb::decode_ber_length(packet, 16, payload_len, len_bytes)) return 0;
    if (packet.size() < 16 + len_bytes + payload_len) return 0;
    std::vector<uint8_t> payload(packet.begin() + 16 + len_bytes,
                                 packet.begin() + 16 + len_bytes + payload_len);
    // Verify checksum
    uint16_t crc_stored = (static_cast<uint16_t>(packet[packet.size()-2]) << 8) | packet.back();
    uint16_t crc_calc = misb::klv_checksum_16(std::vector<uint8_t>(packet.begin(), packet.end()-2));
    if (crc_stored != crc_calc) return 0;
    payload.resize(payload.size()-4);
    KLVSet decoded(false, misb::st0601::ST_ID);
    decoded.decode(payload);

    double ts = 0.0, lat = 0.0, lon = 0.0, ver = 0.0;
    ST_GET(decoded, 0601, UNIX_TIMESTAMP, ts);
    ST_GET(decoded, 0601, SENSOR_LATITUDE, lat);
    ST_GET(decoded, 0601, SENSOR_LONGITUDE, lon);
    ST_GET(decoded, 0601, UAS_LS_VERSION_NUMBER, ver);
    std::cout << "Decoded timestamp: " << ts << '\n';
    std::cout << "Decoded sensor lat/lon: " << lat << ", " << lon << '\n';
    std::cout << "Decoded UAS LS version: " << ver << '\n';

    // Extract the VMTI detections
    KLVSet vmti_decoded(false, misb::st0903::ST_ID);
    KLV_GET_SET(decoded, misb::st0601::VMTI_LOCAL_SET, vmti_decoded);
    std::cout << "Decoded detections:\n";
    for (const auto& node : vmti_decoded.children()) {
        if (auto bytesNode = std::dynamic_pointer_cast<KLVBytes>(node)) {
            if (bytesNode->ul() == misb::st0903::VMTI_VTARGET_SERIES) {
                auto decodedPacks = misb::st0903::decode_vtarget_series(bytesNode->value());
                for (const auto& pack : decodedPacks) {
                    double centroid = get_value(pack.set, misb::st0903::VTARGET_CENTROID);
                    double row = get_value(pack.set, misb::st0903::VTARGET_CENTROID_ROW);
                    double col = get_value(pack.set, misb::st0903::VTARGET_CENTROID_COLUMN);
                    double conf = get_value(pack.set, misb::st0903::VTARGET_CONFIDENCE_LEVEL);
                    double status = get_value(pack.set, misb::st0903::VTARGET_DETECTION_STATUS);
                    std::cout << "  ID " << pack.target_id
                              << " centroid " << centroid
                              << " (row,col)=(" << row << ", " << col << ")"
                              << " conf " << conf
                              << " status " << status << '\n';
                }
            }
        }
    }

    return 0;
}

