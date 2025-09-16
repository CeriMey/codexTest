#include "klv_macros.h"
#include "st0601.h"
#include "st0102.h"
#include "st0903.h"
#include "stanag.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>
#include <limits>

struct Detection {
    uint64_t id;
    double row;
    double column;
    double confidence;
    double status;
};

static double extract_value(const KLVSet& set, const UL& ul) {
    for (const auto& node : set.children()) {
        if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node)) {
            if (leaf->ul() == ul) return leaf->value();
        }
    }
    return std::numeric_limits<double>::quiet_NaN();
}

static std::string extract_string(const KLVSet& set, const UL& ul) {
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
    misb::st0102::register_st0102(reg);
    misb::st0903::register_st0903(reg);

    const double frameWidth = 1280.0;
    const double frameHeight = 720.0;

    std::vector<Detection> detections = {
        {1, 120.0, 300.0, 0.95, 1.0},
        {2, 256.0, 512.0, 0.72, 1.0},
        {3, 400.0, 640.0, 0.55, 0.0},
        {4, 500.0, 850.0, 0.40, 0.0},
        {5, 620.0, 900.0, 0.25, 0.0}
    };

    std::cout << "Input UAV data:" << std::fixed << std::setprecision(2) << '\n';
    std::cout << "  Sensor Latitude: 45.00\n";
    std::cout << "  Sensor Longitude: -75.00\n";
    std::cout << "  Platform Heading: 90.00\n";

    std::cout << "Detections:" << '\n';
    std::vector<misb::st0903::VTargetPack> packs;
    for (const auto& d : detections) {
        std::cout << "  ID " << d.id
                  << " centroid (row,col)=(" << d.row << ", " << d.column << ")"
                  << " confidence " << d.confidence
                  << " status " << d.status << '\n';
        double algorithmRef = (d.id % 2 == 0) ? 2.0 : 1.0;
        double centroidValue = misb::st0903::target_centroid_pixel(d.row, d.column, frameWidth);
        packs.push_back(KLV_VTARGET_PACK(
            d.id,
            KLV_TAG(misb::st0903::VTARGET_CENTROID, centroidValue),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_ROW, d.row),
            KLV_TAG(misb::st0903::VTARGET_CENTROID_COLUMN, d.column),
            KLV_TAG(misb::st0903::VTARGET_CONFIDENCE_LEVEL, d.confidence),
            KLV_TAG(misb::st0903::VTARGET_DETECTION_STATUS, d.status),
            KLV_TAG(misb::st0903::VTARGET_ALGORITHM_ID, algorithmRef)
        ));
    }

    auto algorithmSeries = KLV_ALGORITHM_SERIES(
        KLV_ALGORITHM_SET(
            KLV_LOCAL_LEAF(misb::st0903::ALGORITHM_ID, 1.0),
            KLV_LOCAL_BYTES(misb::st0903::ALGORITHM_NAME, KLV_ASCII_BYTES("MotionNet")),
            KLV_LOCAL_BYTES(misb::st0903::ALGORITHM_VERSION, KLV_ASCII_BYTES("2.1")),
            KLV_LOCAL_LEAF(misb::st0903::ALGORITHM_CLASS, 3.0),
            KLV_LOCAL_LEAF(misb::st0903::ALGORITHM_CONFIDENCE, 0.95)
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
            KLV_LOCAL_LEAF(misb::st0903::ONTOLOGY_ID, 101.0),
            KLV_LOCAL_BYTES(misb::st0903::ONTOLOGY_URI, KLV_ASCII_BYTES("urn:example:vehicle")),
            KLV_LOCAL_LEAF(misb::st0903::ONTOLOGY_CONFIDENCE, 0.82)
        ),
        KLV_ONTOLOGY_SET(
            KLV_LOCAL_LEAF(misb::st0903::ONTOLOGY_ID, 202.0),
            KLV_LOCAL_BYTES(misb::st0903::ONTOLOGY_URI, KLV_ASCII_BYTES("urn:example:person")),
            KLV_LOCAL_LEAF(misb::st0903::ONTOLOGY_CONFIDENCE, 0.91),
            KLV_LOCAL_BYTES(misb::st0903::ONTOLOGY_FAMILY, KLV_ASCII_BYTES("Human"))
        )
    );

    auto series = misb::st0903::encode_vtarget_series(packs);

    KLVSet vmti(false, misb::st0903::ST_ID);
    vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_LS_VERSION, 6.0, true));
    vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_TOTAL_TARGETS_DETECTED,
                                       static_cast<double>(detections.size()), true));
    vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_NUM_TARGETS_REPORTED,
                                       static_cast<double>(detections.size()), true));
    vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_FRAME_WIDTH, frameWidth, true));
    vmti.add(std::make_shared<KLVLeaf>(misb::st0903::VMTI_FRAME_HEIGHT, frameHeight, true));
    vmti.add(std::make_shared<KLVBytes>(misb::st0903::VMTI_VTARGET_SERIES, series, true));
    vmti.add(std::make_shared<KLVBytes>(misb::st0903::VMTI_ALGORITHM_SERIES, algorithmSeries, true));
    vmti.add(std::make_shared<KLVBytes>(misb::st0903::VMTI_ONTOLOGY_SERIES, ontologySeries, true));

    KLVSet data = KLV_LOCAL_DATASET(
        KLV_TAG(misb::st0601::SENSOR_LATITUDE, 45.0),
        KLV_TAG(misb::st0601::SENSOR_LONGITUDE, -75.0),
        KLV_TAG(misb::st0601::PLATFORM_HEADING_ANGLE, 90.0)
    );
    KLV_ADD_BYTES(data, misb::st0601::VMTI_LOCAL_SET, vmti.encode());

    auto bytes = data.encode();
    std::cout << "\nEncoded packet:";
    for (uint8_t b : bytes) {
        std::cout << ' ' << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(b);
    }
    std::cout << std::dec << '\n';

    // Decode
    KLVSet decoded(false, misb::st0601::ST_ID);
    decoded.decode(bytes);
    std::cout << "\nDecoded UAV data:" << '\n';
    KLVSet vmti_decoded(false, misb::st0903::ST_ID);
    for (const auto& node : decoded.children()) {
        if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node)) {
            const UL& ul = leaf->ul();
            if (ul == misb::st0601::SENSOR_LATITUDE) {
                std::cout << "  Sensor Latitude: " << leaf->value() << '\n';
            } else if (ul == misb::st0601::SENSOR_LONGITUDE) {
                std::cout << "  Sensor Longitude: " << leaf->value() << '\n';
            } else if (ul == misb::st0601::PLATFORM_HEADING_ANGLE) {
                std::cout << "  Platform Heading: " << leaf->value() << '\n';
            }
        } else if (auto bytesNode = std::dynamic_pointer_cast<KLVBytes>(node)) {
            if (bytesNode->ul() == misb::st0601::VMTI_LOCAL_SET) {
                vmti_decoded.decode(bytesNode->value());
            }
        }
    }

    std::cout << "Decoded VMTI local set (" << vmti_decoded.children().size()
              << " entries):" << '\n';
    for (const auto& node : vmti_decoded.children()) {
        if (auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node)) {
            const UL& ul = leaf->ul();
            if (ul == misb::st0903::VMTI_LS_VERSION) {
                std::cout << "  LS Version: " << leaf->value() << '\n';
            } else if (ul == misb::st0903::VMTI_TOTAL_TARGETS_DETECTED) {
                std::cout << "  Total Targets Detected: " << leaf->value() << '\n';
            } else if (ul == misb::st0903::VMTI_NUM_TARGETS_REPORTED) {
                std::cout << "  Targets Reported: " << leaf->value() << '\n';
            } else if (ul == misb::st0903::VMTI_FRAME_WIDTH) {
                std::cout << "  Frame Width: " << leaf->value() << '\n';
            } else if (ul == misb::st0903::VMTI_FRAME_HEIGHT) {
                std::cout << "  Frame Height: " << leaf->value() << '\n';
            }
        } else if (auto bytesNode = std::dynamic_pointer_cast<KLVBytes>(node)) {
            if (bytesNode->ul() == misb::st0903::VMTI_VTARGET_SERIES) {
                auto decodedPacks = misb::st0903::decode_vtarget_series(bytesNode->value());
                std::cout << "  Detections:" << '\n';
                for (const auto& pack : decodedPacks) {
                    double centroid = extract_value(pack.set, misb::st0903::VTARGET_CENTROID);
                    double row = extract_value(pack.set, misb::st0903::VTARGET_CENTROID_ROW);
                    double col = extract_value(pack.set, misb::st0903::VTARGET_CENTROID_COLUMN);
                    double conf = extract_value(pack.set, misb::st0903::VTARGET_CONFIDENCE_LEVEL);
                    double status = extract_value(pack.set, misb::st0903::VTARGET_DETECTION_STATUS);
                    double alg = extract_value(pack.set, misb::st0903::VTARGET_ALGORITHM_ID);
                    uint64_t centroidIndex = static_cast<uint64_t>(std::llround(centroid));
                    std::cout << "    ID " << pack.target_id
                              << " centroid " << centroidIndex
                              << " (row,col)=(" << row << ", " << col << ")"
                              << " confidence " << conf
                              << " status " << status
                              << " algorithm " << alg << '\n';
                }
            } else if (bytesNode->ul() == misb::st0903::VMTI_ALGORITHM_SERIES) {
                auto algSets = misb::st0903::decode_algorithm_series(bytesNode->value());
                std::cout << "  Algorithms:" << '\n';
                for (const auto& set : algSets) {
                    double id = extract_value(set, misb::st0903::ALGORITHM_ID);
                    double algClass = extract_value(set, misb::st0903::ALGORITHM_CLASS);
                    double confidence = extract_value(set, misb::st0903::ALGORITHM_CONFIDENCE);
                    std::string name = extract_string(set, misb::st0903::ALGORITHM_NAME);
                    std::string version = extract_string(set, misb::st0903::ALGORITHM_VERSION);
                    std::cout << "    Algorithm " << id
                              << " (" << name;
                    if (!version.empty()) {
                        std::cout << " v" << version;
                    }
                    std::cout << ") class " << algClass
                              << " confidence " << confidence << '\n';
                }
            } else if (bytesNode->ul() == misb::st0903::VMTI_ONTOLOGY_SERIES) {
                auto ontSets = misb::st0903::decode_ontology_series(bytesNode->value());
                std::cout << "  Ontologies:" << '\n';
                for (const auto& set : ontSets) {
                    double id = extract_value(set, misb::st0903::ONTOLOGY_ID);
                    double confidence = extract_value(set, misb::st0903::ONTOLOGY_CONFIDENCE);
                    std::string uri = extract_string(set, misb::st0903::ONTOLOGY_URI);
                    std::string family = extract_string(set, misb::st0903::ONTOLOGY_FAMILY);
                    std::cout << "    Ontology " << id
                              << " uri " << uri
                              << " confidence " << confidence;
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

