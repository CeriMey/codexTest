#include "klv_macros.h"
#include "st0601.h"
#include "st0903.h"
#include "st_common.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

// ---- helpers ----
static double get_value(const KLVSet& set, const UL& ul) {
    for (const auto& node : set.children()) {
        if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node)) {
            if (leaf->ul() == ul) {
                return leaf->value();
            }
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

    // --- mêmes données que "avant" (5 cibles, 95/185 puis +4.5/+3.5, conf 0.45..0.65, status 1/0, algo 2/1) ---
    auto vtargetSeries = KLV_VTARGET_SERIES(
        KLV_VTARGET_PACK(1,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(95.0, 185.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 95.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 185.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.45),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 1.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 2.0)
        ),
        KLV_VTARGET_PACK(2,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(99.5, 188.5, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 99.5),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 188.5),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.50),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 0.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 1.0)
        ),
        KLV_VTARGET_PACK(3,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(104.0, 192.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 104.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 192.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.55),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 1.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 2.0)
        ),
        KLV_VTARGET_PACK(4,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(108.5, 195.5, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 108.5),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 195.5),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.60),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, 0.0),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, 1.0)
        ),
        KLV_VTARGET_PACK(5,
            KLV_TAG(misb::st0903::VTARGET_CENTROID,
                    KLV_PIXEL_NUMBER(113.0, 199.0, frameWidth)),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, 113.0),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, 199.0),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, 0.65),
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

    const double detCount = 5.0;
    auto vmti = KLV_LOCAL_DATASET(
        KLV_TAG(misb::st0903::VMTI_LS_VERSION, 6.0),
        KLV_TAG(misb::st0903::VMTI_TOTAL_TARGETS_DETECTED, detCount),
        KLV_TAG(misb::st0903::VMTI_NUM_TARGETS_REPORTED, detCount),
        KLV_TAG(misb::st0903::VMTI_FRAME_WIDTH, frameWidth),
        KLV_TAG(misb::st0903::VMTI_FRAME_HEIGHT, frameHeight)
    );
    KLV_ADD_BYTES(vmti, misb::st0903::VMTI_VTARGET_SERIES, vtargetSeries);
    KLV_ADD_BYTES(vmti, misb::st0903::VMTI_ALGORITHM_SERIES, algorithmSeries);
    KLV_ADD_BYTES(vmti, misb::st0903::VMTI_ONTOLOGY_SERIES, ontologySeries);

    // mêmes tags ST0601 que l’exemple précédent (FalconEye, EO/IR, WGS-84, lat/lon, version 12)
    auto stanagPacket = STANAG4609_PACKET(
        KLV_ST_ITEM(0601, UNIX_TIMESTAMP, 1700000000.0),
        KLV_TAG(misb::st0601::PLATFORM_DESIGNATION, KLV_ASCII_BYTES("FalconEye")),
        KLV_TAG(misb::st0601::IMAGE_SOURCE_SENSOR, KLV_ASCII_BYTES("EO/IR Sensor")),
        KLV_TAG(misb::st0601::IMAGE_COORDINATE_SYSTEM, KLV_ASCII_BYTES("WGS-84")),
        KLV_ST_ITEM(0601, SENSOR_LATITUDE, 48.8566),
        KLV_ST_ITEM(0601, SENSOR_LONGITUDE, 2.3522),
        KLV_ST_ITEM(0601, UAS_LS_VERSION_NUMBER, 12.0),
        KLV_TAG(misb::st0601::VMTI_LOCAL_SET, vmti)
    );

    // ---- dump hex ----
    std::cout << "Encoded packet:";
    for (uint8_t b : stanagPacket) {
        std::cout << ' ' << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(b);
    }
    std::cout << std::dec << '\n';

    // ---- decode ----
    if (stanagPacket.size() <= 16) {
        return 0;
    }
    size_t payload_len = 0;
    size_t len_bytes = 0;
    if (!misb::decode_ber_length(stanagPacket, 16, payload_len, len_bytes)) {
        return 0;
    }
    if (stanagPacket.size() < 16 + len_bytes + payload_len) {
        return 0;
    }
    std::vector<uint8_t> payload(stanagPacket.begin() + 16 + len_bytes,
                                 stanagPacket.begin() + 16 + len_bytes + payload_len);

    // CRC
    const uint16_t crc_stored = (static_cast<uint16_t>(stanagPacket[stanagPacket.size() - 2]) << 8)
                                | stanagPacket.back();
    const uint16_t crc_calc = misb::klv_checksum_16(
        std::vector<uint8_t>(stanagPacket.begin(), stanagPacket.end() - 2));
    if (crc_stored != crc_calc) {
        return 0;
    }

    // enlever le checksum interne (TLV 1, len 2, data 2)
    if (payload.size() >= 4) {
        payload.resize(payload.size() - 4);
    }

    KLVSet decoded(false, misb::st0601::ST_ID);
    decoded.decode(payload);

    double ts = 0.0;
    double lat = 0.0;
    double lon = 0.0;
    double ver = 0.0;
    ST_GET(decoded, 0601, UNIX_TIMESTAMP, ts);
    ST_GET(decoded, 0601, SENSOR_LATITUDE, lat);
    ST_GET(decoded, 0601, SENSOR_LONGITUDE, lon);
    ST_GET(decoded, 0601, UAS_LS_VERSION_NUMBER, ver);
    std::string platform = get_string(decoded, misb::st0601::PLATFORM_DESIGNATION);
    std::string sensor = get_string(decoded, misb::st0601::IMAGE_SOURCE_SENSOR);
    std::string coordSys = get_string(decoded, misb::st0601::IMAGE_COORDINATE_SYSTEM);

    std::cout << "Decoded timestamp: " << ts << '\n';
    std::cout << "Decoded platform designation: " << platform << '\n';
    std::cout << "Decoded sensor: " << sensor << '\n';
    std::cout << "Decoded coordinate system: " << coordSys << '\n';
    std::cout << "Decoded sensor lat/lon: " << lat << ", " << lon << '\n';
    std::cout << "Decoded UAS LS version: " << ver << '\n';

    // VMTI
    KLVSet vmti_decoded(false, misb::st0903::ST_ID);
    KLV_GET_SET(decoded, misb::st0601::VMTI_LOCAL_SET, vmti_decoded);

    std::cout << "Decoded detections:\n";
    for (const auto& node : vmti_decoded.children()) {
        if (auto bytesNode = std::dynamic_pointer_cast<KLVBytes>(node)) {
            if (bytesNode->ul() == misb::st0903::VMTI_VTARGET_SERIES) {
                auto decodedPacks = misb::st0903::decode_vtarget_series(bytesNode->value());
                for (const auto& pack : decodedPacks) {
                    const double centroid = get_value(pack.set, misb::st0903::VTARGET_CENTROID);
                    const double row = get_value(pack.set, misb::st0903::VTARGET_CENTROID_ROW);
                    const double col = get_value(pack.set, misb::st0903::VTARGET_CENTROID_COLUMN);
                    const double conf = get_value(pack.set, misb::st0903::VTARGET_CONFIDENCE_LEVEL);
                    const double status = get_value(pack.set, misb::st0903::VTARGET_DETECTION_STATUS);
                    const double alg = get_value(pack.set, misb::st0903::VTARGET_ALGORITHM_ID);
                    const uint64_t centroidIndex = static_cast<uint64_t>(std::llround(centroid));
                    std::cout << "  ID " << pack.target_id
                              << " centroid " << centroidIndex
                              << " (row,col)=(" << row << ", " << col << ")"
                              << " conf " << conf
                              << " status " << status
                              << " algorithm " << alg << '\n';
                }
            } else if (bytesNode->ul() == misb::st0903::VMTI_ALGORITHM_SERIES) {
                auto decodedAlgorithms = misb::st0903::decode_algorithm_series(bytesNode->value());
                std::cout << "Algorithms:\n";
                for (const auto& algSet : decodedAlgorithms) {
                    const double algId = get_value(algSet, misb::st0903::ALGORITHM_ID);
                    const double algClass = get_value(algSet, misb::st0903::ALGORITHM_CLASS);
                    const double algConf = get_value(algSet, misb::st0903::ALGORITHM_CONFIDENCE);
                    const std::string name = get_string(algSet, misb::st0903::ALGORITHM_NAME);
                    const std::string verS = get_string(algSet, misb::st0903::ALGORITHM_VERSION);
                    std::cout << "  Algorithm " << algId
                              << " (" << name;
                    if (!verS.empty()) {
                        std::cout << " v" << verS;
                    }
                    std::cout << ") class " << algClass
                              << " confidence " << algConf << '\n';
                }
            } else if (bytesNode->ul() == misb::st0903::VMTI_ONTOLOGY_SERIES) {
                auto decodedOntologies = misb::st0903::decode_ontology_series(bytesNode->value());
                std::cout << "Ontologies:\n";
                for (const auto& ontSet : decodedOntologies) {
                    const double ontId = get_value(ontSet, misb::st0903::ONTOLOGY_ID);
                    const double ontConf = get_value(ontSet, misb::st0903::ONTOLOGY_CONFIDENCE);
                    const std::string uri = get_string(ontSet, misb::st0903::ONTOLOGY_URI);
                    const std::string family = get_string(ontSet, misb::st0903::ONTOLOGY_FAMILY);
                    std::cout << "  Ontology " << ontId
                              << " uri " << uri
                              << " confidence " << ontConf;
                    if (!family.empty()) {
                        std::cout << " family " << family;
                    }
                    std::cout << '\n';
                }
            }
        }
    }

    return 0;
}
