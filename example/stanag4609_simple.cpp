#include "klv_macros.h"
#include "st0601.h"
#include "st0903.h"
#include "st_common.h"
#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <limits>
#include <string>
#include <cmath>

static double get_value(const KLVSet& set, const UL& ul) {
    for (const auto& node : set.children()) {
        if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node)) {
            if (leaf->ul() == ul) return leaf->value();
        }
    }
    return std::numeric_limits<double>::quiet_NaN();
}

static std::string get_string(const KLVSet& set, const UL& ul) {
    for (const auto& node : set.children()) {
        if (auto bytes = std::dynamic_pointer_cast<KLVBytes>(node)) {
            if (bytes->ul() == ul) {
                return std::string(bytes->value().begin(), bytes->value().end());
            }
        }
    }
    return {};
}

int main() {
    auto& reg = KLVRegistry::instance();
    misb::st0601::register_st0601(reg);
    misb::st0903::register_st0903(reg);

    const double frameWidth = 1920.0;
    const double frameHeight = 1080.0;

    // Build a VMTI local set with 20 detections
    std::vector<misb::st0903::VTargetPack> packs;
    packs.reserve(20);
    for (int i = 1; i <= 20; ++i) {
        double row = 100.0 + i * 5.0;
        double col = 200.0 + i * 3.0;
        double confidence = 0.4 + 0.01 * static_cast<double>(i);
        double status = (i % 2 == 0) ? 1.0 : 0.0;
        double algorithmRef = (i % 2 == 0) ? 2.0 : 1.0;
        packs.push_back(KLV_VTARGET_PACK(
            i,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(row, col, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, row),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, col),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, confidence),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, status),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, algorithmRef)
        ));
    }

    auto series = misb::st0903::encode_vtarget_series(packs);

    auto algorithmSeries = KLV_ALGORITHM_SERIES(
        KLV_ALGORITHM_SET(
            KLV_LOCAL_LEAF(misb::st0903::ALGORITHM_ID, 1.0),
            KLV_LOCAL_BYTES(misb::st0903::ALGORITHM_NAME, KLV_ASCII_BYTES("MotionNet")),
            KLV_LOCAL_BYTES(misb::st0903::ALGORITHM_VERSION, KLV_ASCII_BYTES("2.1")),
            KLV_LOCAL_LEAF(misb::st0903::ALGORITHM_CLASS, 3.0),
            KLV_LOCAL_LEAF(misb::st0903::ALGORITHM_CONFIDENCE, 0.92)
        ),
        KLV_ALGORITHM_SET(
            KLV_LOCAL_LEAF(misb::st0903::ALGORITHM_ID, 2.0),
            KLV_LOCAL_BYTES(misb::st0903::ALGORITHM_NAME, KLV_ASCII_BYTES("TrackerAI")),
            KLV_LOCAL_BYTES(misb::st0903::ALGORITHM_VERSION, KLV_ASCII_BYTES("1.4")),
            KLV_LOCAL_LEAF(misb::st0903::ALGORITHM_CLASS, 4.0),
            KLV_LOCAL_LEAF(misb::st0903::ALGORITHM_CONFIDENCE, 0.88)
        )
    );

    auto ontologySeries = KLV_ONTOLOGY_SERIES(
        KLV_ONTOLOGY_SET(
            KLV_LOCAL_LEAF(misb::st0903::ONTOLOGY_ID, 1001.0),
            KLV_LOCAL_BYTES(misb::st0903::ONTOLOGY_URI, KLV_ASCII_BYTES("urn:example:vehicle")),
            KLV_LOCAL_BYTES(misb::st0903::ONTOLOGY_FAMILY, KLV_ASCII_BYTES("Vehicle")),
            KLV_LOCAL_LEAF(misb::st0903::ONTOLOGY_CONFIDENCE, 0.81)
        ),
        KLV_ONTOLOGY_SET(
            KLV_LOCAL_LEAF(misb::st0903::ONTOLOGY_ID, 1002.0),
            KLV_LOCAL_BYTES(misb::st0903::ONTOLOGY_URI, KLV_ASCII_BYTES("urn:example:person")),
            KLV_LOCAL_BYTES(misb::st0903::ONTOLOGY_FAMILY, KLV_ASCII_BYTES("Human")),
            KLV_LOCAL_LEAF(misb::st0903::ONTOLOGY_CONFIDENCE, 0.93)
        )
    );

    const double detectionCount = 20.0;
    KLVSet vmti = KLV_LOCAL_DATASET(
        KLV_TAG(misb::st0903::VMTI_LS_VERSION, 6.0),
        KLV_TAG(misb::st0903::VMTI_TOTAL_TARGETS_DETECTED, detectionCount),
        KLV_TAG(misb::st0903::VMTI_NUM_TARGETS_REPORTED, detectionCount),
        KLV_TAG(misb::st0903::VMTI_FRAME_WIDTH, frameWidth),
        KLV_TAG(misb::st0903::VMTI_FRAME_HEIGHT, frameHeight)
    );
    KLV_ADD_BYTES(vmti, misb::st0903::VMTI_VTARGET_SERIES, series);
    KLV_ADD_BYTES(vmti, misb::st0903::VMTI_ALGORITHM_SERIES, algorithmSeries);
    KLV_ADD_BYTES(vmti, misb::st0903::VMTI_ONTOLOGY_SERIES, ontologySeries);

    // Compose the STANAG 4609 packet (UAS LS version tag added automatically)
    auto packet = STANAG4609_PACKET(
        KLV_ST_ITEM(0601, UNIX_TIMESTAMP, 1700000000.0),
        KLV_ST_ITEM(0601, PLATFORM_DESIGNATION, "FalconEye"),
        KLV_ST_ITEM(0601, IMAGE_SOURCE_SENSOR, "EO/IR Sensor"),
        KLV_ST_ITEM(0601, IMAGE_COORDINATE_SYSTEM, "WGS-84"),
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
    std::string platformDesignation;
    std::string imageSensor;
    std::string coordSystem;
    ST_GET(decoded, 0601, UNIX_TIMESTAMP, ts);
    platformDesignation = get_string(decoded, misb::st0601::PLATFORM_DESIGNATION);
    imageSensor = get_string(decoded, misb::st0601::IMAGE_SOURCE_SENSOR);
    coordSystem = get_string(decoded, misb::st0601::IMAGE_COORDINATE_SYSTEM);
    ST_GET(decoded, 0601, SENSOR_LATITUDE, lat);
    ST_GET(decoded, 0601, SENSOR_LONGITUDE, lon);
    ST_GET(decoded, 0601, UAS_LS_VERSION_NUMBER, ver);
    std::cout << "Decoded timestamp: " << ts << '\n';
    std::cout << "Decoded platform designation: " << platformDesignation << '\n';
    std::cout << "Decoded sensor: " << imageSensor << '\n';
    std::cout << "Decoded coordinate system: " << coordSystem << '\n';
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
                    double algorithm = get_value(pack.set, misb::st0903::VTARGET_ALGORITHM_ID);
                    uint64_t centroidIndex = static_cast<uint64_t>(std::llround(centroid));
                    std::cout << "  ID " << pack.target_id
                              << " centroid " << centroidIndex
                              << " (row,col)=(" << row << ", " << col << ")"
                              << " conf " << conf
                              << " status " << status
                              << " algorithm " << algorithm << '\n';
                }
            } else if (bytesNode->ul() == misb::st0903::VMTI_ALGORITHM_SERIES) {
                auto decodedAlgorithms = misb::st0903::decode_algorithm_series(bytesNode->value());
                std::cout << "Algorithms:\n";
                for (const auto& algSet : decodedAlgorithms) {
                    double algId = get_value(algSet, misb::st0903::ALGORITHM_ID);
                    double algClass = get_value(algSet, misb::st0903::ALGORITHM_CLASS);
                    double algConfidence = get_value(algSet, misb::st0903::ALGORITHM_CONFIDENCE);
                    std::string algName = get_string(algSet, misb::st0903::ALGORITHM_NAME);
                    std::string algVersion = get_string(algSet, misb::st0903::ALGORITHM_VERSION);
                    std::cout << "  Algorithm " << algId
                              << " (" << algName;
                    if (!algVersion.empty()) {
                        std::cout << " v" << algVersion;
                    }
                    std::cout << ") class " << algClass
                              << " confidence " << algConfidence << '\n';
                }
            } else if (bytesNode->ul() == misb::st0903::VMTI_ONTOLOGY_SERIES) {
                auto decodedOntologies = misb::st0903::decode_ontology_series(bytesNode->value());
                std::cout << "Ontologies:\n";
                for (const auto& ontSet : decodedOntologies) {
                    double ontId = get_value(ontSet, misb::st0903::ONTOLOGY_ID);
                    double ontConfidence = get_value(ontSet, misb::st0903::ONTOLOGY_CONFIDENCE);
                    std::string ontUri = get_string(ontSet, misb::st0903::ONTOLOGY_URI);
                    std::string ontFamily = get_string(ontSet, misb::st0903::ONTOLOGY_FAMILY);
                    std::cout << "  Ontology " << ontId
                              << " uri " << ontUri
                              << " confidence " << ontConfidence;
                    if (!ontFamily.empty()) {
                        std::cout << " family " << ontFamily;
                    }
                    std::cout << '\n';
                }
            }
        }
    }

    return 0;
}

