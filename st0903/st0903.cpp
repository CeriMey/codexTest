#include "st0903.h"

namespace misb {
namespace st0903 {

// Dummy UL values for demonstration purposes
const UL VMTI_TARGET_ID        = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x00};
const UL VMTI_DETECTION_STATUS = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x01};
const UL VMTI_DETECTION_PROBABILITY = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x02};
const UL VMTI_TRACKER_ID        = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x03};
const UL VMTI_CLASS_ID          = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x04};
const UL VMTI_TOTAL_TARGETS_DETECTED = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x05};
const UL VMTI_NUM_TARGETS_REPORTED  = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x06};
const UL VMTI_FRAME_WIDTH           = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x07};
const UL VMTI_FRAME_HEIGHT          = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x08};
const UL VMTI_SOURCE_SENSOR         = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x09};
const UL VMTI_HORIZONTAL_FOV        = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x0A};
const UL VMTI_VERTICAL_FOV          = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x0B};
const UL VMTI_MIIS_ID               = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x0C};
const UL VMTI_CENTROID_ROW          = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x0D};
const UL VMTI_CENTROID_COL          = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x0E};
const UL VMTI_ALGORITHM_ID          = {0x06,0x0E,0x2B,0x34,0x02,0x0B,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x00,0x00,0x0F};

void register_st0903(KLVRegistry& reg) {
    // VMTI Target ID: uint16 big-endian
    reg.register_ul(VMTI_TARGET_ID, {
        [](double id) {
            uint16_t raw = static_cast<uint16_t>(id);
            return std::vector<uint8_t>{
                static_cast<uint8_t>((raw >> 8) & 0xFF),
                static_cast<uint8_t>(raw & 0xFF)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = (static_cast<uint16_t>(bytes[0]) << 8) | bytes[1];
            return static_cast<double>(raw);
        }
    });

    // VMTI Detection Status: enumeration -> uint8
    reg.register_ul(VMTI_DETECTION_STATUS, {
        [](double status) {
            uint8_t raw = static_cast<uint8_t>(status);
            return std::vector<uint8_t>{raw};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return static_cast<double>(bytes[0]);
        }
    });

    // VMTI Detection Probability: [0,1] -> uint8
    reg.register_ul(VMTI_DETECTION_PROBABILITY, {
        [](double prob) {
            if (prob < 0.0) prob = 0.0;
            if (prob > 1.0) prob = 1.0;
            uint8_t raw = static_cast<uint8_t>(prob * 255.0);
            return std::vector<uint8_t>{raw};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return static_cast<double>(bytes[0]) / 255.0;
        }
    });

    // VMTI Tracker ID: uint16 big-endian
    reg.register_ul(VMTI_TRACKER_ID, {
        [](double id) {
            uint16_t raw = static_cast<uint16_t>(id);
            return std::vector<uint8_t>{
                static_cast<uint8_t>((raw >> 8) & 0xFF),
                static_cast<uint8_t>(raw & 0xFF)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = (static_cast<uint16_t>(bytes[0]) << 8) | bytes[1];
            return static_cast<double>(raw);
        }
    });

    // VMTI Class ID: uint8
    reg.register_ul(VMTI_CLASS_ID, {
        [](double cls) {
            uint8_t raw = static_cast<uint8_t>(cls);
            return std::vector<uint8_t>{raw};
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 1) return 0.0;
            return static_cast<double>(bytes[0]);
        }
    });

    // VMTI Total Number of Targets Detected: uint16 big-endian
    reg.register_ul(VMTI_TOTAL_TARGETS_DETECTED, {
        [](double count) {
            uint16_t raw = static_cast<uint16_t>(count);
            return std::vector<uint8_t>{
                static_cast<uint8_t>((raw >> 8) & 0xFF),
                static_cast<uint8_t>(raw & 0xFF)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = (static_cast<uint16_t>(bytes[0]) << 8) | bytes[1];
            return static_cast<double>(raw);
        }
    });

    // VMTI Number of Targets Reported: uint16 big-endian
    reg.register_ul(VMTI_NUM_TARGETS_REPORTED, {
        [](double count) {
            uint16_t raw = static_cast<uint16_t>(count);
            return std::vector<uint8_t>{
                static_cast<uint8_t>((raw >> 8) & 0xFF),
                static_cast<uint8_t>(raw & 0xFF)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = (static_cast<uint16_t>(bytes[0]) << 8) | bytes[1];
            return static_cast<double>(raw);
        }
    });

    // VMTI Frame Width: uint16 big-endian
    reg.register_ul(VMTI_FRAME_WIDTH, {
        [](double width) {
            uint16_t raw = static_cast<uint16_t>(width);
            return std::vector<uint8_t>{
                static_cast<uint8_t>((raw >> 8) & 0xFF),
                static_cast<uint8_t>(raw & 0xFF)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = (static_cast<uint16_t>(bytes[0]) << 8) | bytes[1];
            return static_cast<double>(raw);
        }
    });

    // VMTI Frame Height: uint16 big-endian
    reg.register_ul(VMTI_FRAME_HEIGHT, {
        [](double height) {
            uint16_t raw = static_cast<uint16_t>(height);
            return std::vector<uint8_t>{
                static_cast<uint8_t>((raw >> 8) & 0xFF),
                static_cast<uint8_t>(raw & 0xFF)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = (static_cast<uint16_t>(bytes[0]) << 8) | bytes[1];
            return static_cast<double>(raw);
        }
    });

    // VMTI Source Sensor: uint16 big-endian
    reg.register_ul(VMTI_SOURCE_SENSOR, {
        [](double sensor) {
            uint16_t raw = static_cast<uint16_t>(sensor);
            return std::vector<uint8_t>{
                static_cast<uint8_t>((raw >> 8) & 0xFF),
                static_cast<uint8_t>(raw & 0xFF)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = (static_cast<uint16_t>(bytes[0]) << 8) | bytes[1];
            return static_cast<double>(raw);
        }
    });

    // VMTI Horizontal Field of View: uint16 big-endian
    reg.register_ul(VMTI_HORIZONTAL_FOV, {
        [](double fov) {
            uint16_t raw = static_cast<uint16_t>(fov);
            return std::vector<uint8_t>{
                static_cast<uint8_t>((raw >> 8) & 0xFF),
                static_cast<uint8_t>(raw & 0xFF)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = (static_cast<uint16_t>(bytes[0]) << 8) | bytes[1];
            return static_cast<double>(raw);
        }
    });

    // VMTI Vertical Field of View: uint16 big-endian
    reg.register_ul(VMTI_VERTICAL_FOV, {
        [](double fov) {
            uint16_t raw = static_cast<uint16_t>(fov);
            return std::vector<uint8_t>{
                static_cast<uint8_t>((raw >> 8) & 0xFF),
                static_cast<uint8_t>(raw & 0xFF)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = (static_cast<uint16_t>(bytes[0]) << 8) | bytes[1];
            return static_cast<double>(raw);
        }
    });

    // VMTI MIIS ID: uint16 big-endian
    reg.register_ul(VMTI_MIIS_ID, {
        [](double id) {
            uint16_t raw = static_cast<uint16_t>(id);
            return std::vector<uint8_t>{
                static_cast<uint8_t>((raw >> 8) & 0xFF),
                static_cast<uint8_t>(raw & 0xFF)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = (static_cast<uint16_t>(bytes[0]) << 8) | bytes[1];
            return static_cast<double>(raw);
        }
    });

    // VMTI Centroid Row: uint16 big-endian
    reg.register_ul(VMTI_CENTROID_ROW, {
        [](double row) {
            uint16_t raw = static_cast<uint16_t>(row);
            return std::vector<uint8_t>{
                static_cast<uint8_t>((raw >> 8) & 0xFF),
                static_cast<uint8_t>(raw & 0xFF)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = (static_cast<uint16_t>(bytes[0]) << 8) | bytes[1];
            return static_cast<double>(raw);
        }
    });

    // VMTI Centroid Column: uint16 big-endian
    reg.register_ul(VMTI_CENTROID_COL, {
        [](double col) {
            uint16_t raw = static_cast<uint16_t>(col);
            return std::vector<uint8_t>{
                static_cast<uint8_t>((raw >> 8) & 0xFF),
                static_cast<uint8_t>(raw & 0xFF)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = (static_cast<uint16_t>(bytes[0]) << 8) | bytes[1];
            return static_cast<double>(raw);
        }
    });

    // VMTI Algorithm ID: uint16 big-endian
    reg.register_ul(VMTI_ALGORITHM_ID, {
        [](double id) {
            uint16_t raw = static_cast<uint16_t>(id);
            return std::vector<uint8_t>{
                static_cast<uint8_t>((raw >> 8) & 0xFF),
                static_cast<uint8_t>(raw & 0xFF)
            };
        },
        [](const std::vector<uint8_t>& bytes) {
            if (bytes.size() != 2) return 0.0;
            uint16_t raw = (static_cast<uint16_t>(bytes[0]) << 8) | bytes[1];
            return static_cast<double>(raw);
        }
    });
}

} // namespace st0903
} // namespace misb

