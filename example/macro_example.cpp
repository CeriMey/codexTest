#include "klv_macros.h"
#include "st0601.h"
#include "st0903.h"
#include "st_common.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <string>
#include <cmath>
#include <memory>

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

    auto vtargetSeries = KLV_VTARGET_SERIES(
        KLV_VTARGET_PACK(1,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(105.0, 203.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 105.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 203.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.41),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 0.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 1.0)
        ),
        KLV_VTARGET_PACK(2,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(110.0, 206.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 110.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 206.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.42),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 1.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 2.0)
        ),
        KLV_VTARGET_PACK(3,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(115.0, 209.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 115.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 209.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.43),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 0.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 1.0)
        ),
        KLV_VTARGET_PACK(4,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(120.0, 212.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 120.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 212.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.44),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 1.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 2.0)
        ),
        KLV_VTARGET_PACK(5,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(125.0, 215.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 125.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 215.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.45),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 0.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 1.0)
        ),
        KLV_VTARGET_PACK(6,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(130.0, 218.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 130.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 218.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.46),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 1.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 2.0)
        ),
        KLV_VTARGET_PACK(7,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(135.0, 221.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 135.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 221.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.47),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 0.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 1.0)
        ),
        KLV_VTARGET_PACK(8,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(140.0, 224.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 140.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 224.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.48),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 1.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 2.0)
        ),
        KLV_VTARGET_PACK(9,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(145.0, 227.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 145.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 227.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.49),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 0.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 1.0)
        ),
        KLV_VTARGET_PACK(10,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(150.0, 230.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 150.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 230.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.5),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 1.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 2.0)
        ),
        KLV_VTARGET_PACK(11,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(155.0, 233.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 155.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 233.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.51),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 0.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 1.0)
        ),
        KLV_VTARGET_PACK(12,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(160.0, 236.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 160.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 236.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.52),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 1.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 2.0)
        ),
        KLV_VTARGET_PACK(13,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(165.0, 239.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 165.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 239.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.53),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 0.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 1.0)
        ),
        KLV_VTARGET_PACK(14,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(170.0, 242.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 170.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 242.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.54),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 1.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 2.0)
        ),
        KLV_VTARGET_PACK(15,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(175.0, 245.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 175.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 245.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.55),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 0.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 1.0)
        ),
        KLV_VTARGET_PACK(16,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(180.0, 248.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 180.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 248.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.56),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 1.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 2.0)
        ),
        KLV_VTARGET_PACK(17,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(185.0, 251.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 185.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 251.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.57),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 0.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 1.0)
        ),
        KLV_VTARGET_PACK(18,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(190.0, 254.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 190.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 254.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.58),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 1.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 2.0)
        ),
        KLV_VTARGET_PACK(19,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(195.0, 257.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 195.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 257.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.59),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 0.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 1.0)
        ),
        KLV_VTARGET_PACK(20,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(200.0, 260.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 200.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 260.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.6),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 1.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 2.0)
        )
    );

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
    auto vmti = KLV_LOCAL_DATASET(
        KLV_TAG(misb::st0903::VMTI_LS_VERSION, 6.0),
        KLV_TAG(misb::st0903::VMTI_TOTAL_TARGETS_DETECTED, detectionCount),
        KLV_TAG(misb::st0903::VMTI_NUM_TARGETS_REPORTED, detectionCount),
        KLV_TAG(misb::st0903::VMTI_FRAME_WIDTH, frameWidth),
        KLV_TAG(misb::st0903::VMTI_FRAME_HEIGHT, frameHeight)
    );
    KLV_ADD_BYTES(vmti, misb::st0903::VMTI_VTARGET_SERIES, vtargetSeries);
    KLV_ADD_BYTES(vmti, misb::st0903::VMTI_ALGORITHM_SERIES, algorithmSeries);
    KLV_ADD_BYTES(vmti, misb::st0903::VMTI_ONTOLOGY_SERIES, ontologySeries);

    auto stanagPacket = STANAG4609_PACKET(
        KLV_ST_ITEM(0601, UNIX_TIMESTAMP, 1700000000.0),
        KLV_ST_ITEM(0601, SENSOR_LATITUDE, 48.8566),
        KLV_ST_ITEM(0601, SENSOR_LONGITUDE, 2.3522),
        KLV_TAG(misb::st0601::VMTI_LOCAL_SET, vmti)
    );

    std::cout << "Encoded packet:";
    for (uint8_t b : stanagPacket) {
        std::cout << ' ' << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(b);
    }
    std::cout << std::dec << '\n';

    if (stanagPacket.size() <= 16) return 0;
    size_t payload_len = 0, len_bytes = 0;
    if (!misb::decode_ber_length(stanagPacket, 16, payload_len, len_bytes)) return 0;
    if (stanagPacket.size() < 16 + len_bytes + payload_len) return 0;
    std::vector<uint8_t> payload(stanagPacket.begin() + 16 + len_bytes,
                                 stanagPacket.begin() + 16 + len_bytes + payload_len);
    uint16_t crc_stored = (static_cast<uint16_t>(stanagPacket[stanagPacket.size()-2]) << 8)
                          | stanagPacket.back();
    uint16_t crc_calc =
        misb::klv_checksum_16(std::vector<uint8_t>(stanagPacket.begin(), stanagPacket.end()-2));
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

