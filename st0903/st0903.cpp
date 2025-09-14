#include "st0903.h"

namespace misb {
namespace st0903 {

void register_st0903(KLVRegistry& reg) {
    // VMTI Target ID: uint16 big-endian
    reg.register_ul(VMTI_TARGET_ID, {
        [](double id) { return pack_be<uint16_t>(static_cast<uint16_t>(id)); },
        [](const std::vector<uint8_t>& bytes) {
            uint16_t raw{};
            if (!unpack_be<uint16_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });

    // VMTI Detection Status: enumeration -> uint8
    reg.register_ul(VMTI_DETECTION_STATUS, {
        [](double status) { return pack_be<uint8_t>(static_cast<uint8_t>(status)); },
        [](const std::vector<uint8_t>& bytes) {
            uint8_t raw{};
            if (!unpack_be<uint8_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });

    // VMTI Detection Probability: [0,1] -> uint8
    reg.register_ul(VMTI_DETECTION_PROBABILITY, {
        [](double prob) {
            if (prob < 0.0) prob = 0.0;
            if (prob > 1.0) prob = 1.0;
            return pack_be<uint8_t>(static_cast<uint8_t>(prob * 255.0));
        },
        [](const std::vector<uint8_t>& bytes) {
            uint8_t raw{};
            if (!unpack_be<uint8_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw) / 255.0;
        }
    });

    // VMTI Tracker ID: uint16 big-endian
    reg.register_ul(VMTI_TRACKER_ID, {
        [](double id) { return pack_be<uint16_t>(static_cast<uint16_t>(id)); },
        [](const std::vector<uint8_t>& bytes) {
            uint16_t raw{};
            if (!unpack_be<uint16_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });

    // VMTI Class ID: uint8
    reg.register_ul(VMTI_CLASS_ID, {
        [](double cls) { return pack_be<uint8_t>(static_cast<uint8_t>(cls)); },
        [](const std::vector<uint8_t>& bytes) {
            uint8_t raw{};
            if (!unpack_be<uint8_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });

    // VMTI Total Number of Targets Detected: uint16 big-endian
    reg.register_ul(VMTI_TOTAL_TARGETS_DETECTED, {
        [](double count) { return pack_be<uint16_t>(static_cast<uint16_t>(count)); },
        [](const std::vector<uint8_t>& bytes) {
            uint16_t raw{};
            if (!unpack_be<uint16_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });

    // VMTI Number of Targets Reported: uint16 big-endian
    reg.register_ul(VMTI_NUM_TARGETS_REPORTED, {
        [](double count) { return pack_be<uint16_t>(static_cast<uint16_t>(count)); },
        [](const std::vector<uint8_t>& bytes) {
            uint16_t raw{};
            if (!unpack_be<uint16_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });

    // VMTI Frame Width: uint16 big-endian
    reg.register_ul(VMTI_FRAME_WIDTH, {
        [](double width) { return pack_be<uint16_t>(static_cast<uint16_t>(width)); },
        [](const std::vector<uint8_t>& bytes) {
            uint16_t raw{};
            if (!unpack_be<uint16_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });

    // VMTI Frame Height: uint16 big-endian
    reg.register_ul(VMTI_FRAME_HEIGHT, {
        [](double height) { return pack_be<uint16_t>(static_cast<uint16_t>(height)); },
        [](const std::vector<uint8_t>& bytes) {
            uint16_t raw{};
            if (!unpack_be<uint16_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });

    // VMTI Source Sensor: uint16 big-endian
    reg.register_ul(VMTI_SOURCE_SENSOR, {
        [](double sensor) { return pack_be<uint16_t>(static_cast<uint16_t>(sensor)); },
        [](const std::vector<uint8_t>& bytes) {
            uint16_t raw{};
            if (!unpack_be<uint16_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });

    // VMTI Horizontal Field of View: uint16 big-endian
    reg.register_ul(VMTI_HORIZONTAL_FOV, {
        [](double fov) { return pack_be<uint16_t>(static_cast<uint16_t>(fov)); },
        [](const std::vector<uint8_t>& bytes) {
            uint16_t raw{};
            if (!unpack_be<uint16_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });

    // VMTI Vertical Field of View: uint16 big-endian
    reg.register_ul(VMTI_VERTICAL_FOV, {
        [](double fov) { return pack_be<uint16_t>(static_cast<uint16_t>(fov)); },
        [](const std::vector<uint8_t>& bytes) {
            uint16_t raw{};
            if (!unpack_be<uint16_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });

    // VMTI MIIS ID: uint16 big-endian
    reg.register_ul(VMTI_MIIS_ID, {
        [](double id) { return pack_be<uint16_t>(static_cast<uint16_t>(id)); },
        [](const std::vector<uint8_t>& bytes) {
            uint16_t raw{};
            if (!unpack_be<uint16_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });

    // VMTI Centroid Row: uint16 big-endian
    reg.register_ul(VMTI_CENTROID_ROW, {
        [](double row) { return pack_be<uint16_t>(static_cast<uint16_t>(row)); },
        [](const std::vector<uint8_t>& bytes) {
            uint16_t raw{};
            if (!unpack_be<uint16_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });

    // VMTI Centroid Column: uint16 big-endian
    reg.register_ul(VMTI_CENTROID_COL, {
        [](double col) { return pack_be<uint16_t>(static_cast<uint16_t>(col)); },
        [](const std::vector<uint8_t>& bytes) {
            uint16_t raw{};
            if (!unpack_be<uint16_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });

    // VMTI Algorithm ID: uint16 big-endian
    reg.register_ul(VMTI_ALGORITHM_ID, {
        [](double id) { return pack_be<uint16_t>(static_cast<uint16_t>(id)); },
        [](const std::vector<uint8_t>& bytes) {
            uint16_t raw{};
            if (!unpack_be<uint16_t>(bytes, raw)) return 0.0;
            return static_cast<double>(raw);
        }
    });
}

} // namespace st0903
} // namespace misb

