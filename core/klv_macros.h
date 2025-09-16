#pragma once

#include "klv.h"
#include "stanag.h"
#include "st0601.h"
#include "st0903.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

namespace misb {

namespace detail {

inline std::vector<uint8_t> to_ascii_bytes_impl(const char* text, size_t length) {
    if (!text || length == 0) {
        return {};
    }
    std::vector<uint8_t> bytes;
    bytes.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        bytes.push_back(static_cast<uint8_t>(static_cast<unsigned char>(text[i])));
    }
    return bytes;
}

} // namespace detail

inline std::vector<uint8_t> to_ascii_bytes(const std::string& text) {
    return detail::to_ascii_bytes_impl(text.data(), text.size());
}

inline std::vector<uint8_t> to_ascii_bytes(const char* text) {
    if (!text) {
        return {};
    }
    return detail::to_ascii_bytes_impl(text, std::strlen(text));
}

template <size_t N>
inline std::vector<uint8_t> to_ascii_bytes(const char (&text)[N]) {
    return detail::to_ascii_bytes_impl(text, N ? N - 1 : 0);
}

namespace st0903 {

inline uint64_t target_centroid_pixel_index(double row,
                                            double column,
                                            double frame_width) {
    if (!std::isfinite(row) || !std::isfinite(column) ||
        !std::isfinite(frame_width) || frame_width <= 0.0) {
        return 0u;
    }
    const double raw_index = column + ((row - 1.0) * frame_width);
    const double clamped_min = std::max(raw_index, 1.0);
    const double max_value = static_cast<double>(0xFFFFFFFFFFFFull);
    const double clamped = std::min(clamped_min, max_value);
    return static_cast<uint64_t>(std::llround(clamped));
}

inline double target_centroid_pixel(double row,
                                    double column,
                                    double frame_width) {
    return static_cast<double>(target_centroid_pixel_index(row, column, frame_width));
}

} // namespace st0903

} // namespace misb

#define KLV_ASCII_BYTES(text) ::misb::to_ascii_bytes(text)
#define KLV_PIXEL_NUMBER(row, column, frame_width) \
    ::misb::st0903::target_centroid_pixel(row, column, frame_width)

// Basic tag and set construction
#define KLV_TAG(tag, value) stanag::TagValue(tag, value)
#define KLV_SET(...) stanag::create_dataset({__VA_ARGS__})
#define KLV_LOCAL_DATASET(...) stanag::create_dataset({__VA_ARGS__}, false)
#define KLV_DATASET(tag, ...) KLV_TAG(tag, KLV_SET(__VA_ARGS__))

#define KLV_LOCAL_LEAF(tag, value) std::make_shared<KLVLeaf>(tag, value, true)
#define KLV_LOCAL_BYTES(tag, bytes) std::make_shared<KLVBytes>(tag, bytes, true)

#define KLV_VTARGET_SERIES(...) misb::st0903::encode_vtarget_series({__VA_ARGS__})
#define KLV_ALGORITHM_SET(...) \
    misb::st0903::make_local_set(misb::st0903::ALGORITHM_ST_ID, {__VA_ARGS__})
#define KLV_ONTOLOGY_SET(...) \
    misb::st0903::make_local_set(misb::st0903::ONTOLOGY_ST_ID, {__VA_ARGS__})
#define KLV_ALGORITHM_SERIES(...) misb::st0903::encode_algorithm_series({__VA_ARGS__})
#define KLV_ONTOLOGY_SERIES(...) misb::st0903::encode_ontology_series({__VA_ARGS__})

// Helper to assemble a VTarget pack with local-tag encoding
#define KLV_VTARGET_PACK(ID, ...)                                                      \
    misb::st0903::VTargetPack{static_cast<uint64_t>(ID), KLV_LOCAL_DATASET(__VA_ARGS__)}

// ST helper to embed nested datasets
#define KLV_ST_DATASET(ST, TAG, ...) \
    KLV_DATASET(KLV_ST_TAG(ST, TAG), __VA_ARGS__)

// Helpers to reference MISB namespaces generically
#define KLV_ST_TAG(ST, TAG) misb::st##ST::TAG
#define KLV_ST_ITEM(ST, TAG, VALUE) KLV_TAG(KLV_ST_TAG(ST, TAG), VALUE)

// Compose a complete STANAG 4609 packet from tag/value pairs
// Automatically appends the UAS LS version number as the final tag
// so callers do not need to specify it explicitly.
#define STANAG4609_PACKET(...) \
    stanag::create_stanag4609_packet({__VA_ARGS__, \
                                      KLV_ST_ITEM(0601, UAS_LS_VERSION_NUMBER, 12.0)})

// Dataset manipulation helpers
#define KLV_ADD_LEAF(dataset, tag, value) \
    dataset.add(std::make_shared<KLVLeaf>(tag, value))
#define KLV_ADD_BYTES(dataset, tag, bytes)                                             \
    do {                                                                              \
        bool __klv_use_tag = !(dataset).uses_ul_keys();                               \
        (dataset).add(std::make_shared<KLVBytes>(tag, bytes, __klv_use_tag));         \
    } while (0)

// Decode and extract values
#define KLV_DECODE_SET(dataset, bytes) \
    dataset.decode(bytes)
#define KLV_GET(dataset, tag, out)                                         \
    for (const auto& node : dataset.children()) {                          \
        auto leaf = std::dynamic_pointer_cast<KLVLeaf>(node);              \
        if (leaf && leaf->ul() == tag) {                                   \
            out = leaf->value();                                           \
        }                                                                  \
    }

#define ST_GET(dataset, ST, TAG, out) \
    KLV_GET(dataset, KLV_ST_TAG(ST, TAG), out)

#define KLV_GET_SET(dataset, tag, out)                                     \
    for (const auto& node : dataset.children()) {                          \
        auto bytes = std::dynamic_pointer_cast<KLVBytes>(node);            \
        if (bytes && bytes->ul() == tag) {                                 \
            out.decode(bytes->value());                                    \
        }                                                                  \
    }

#define ST_GET_SET(dataset, ST, TAG, out) \
    KLV_GET_SET(dataset, KLV_ST_TAG(ST, TAG), out)

// Convenience macro to decode a nested KLV set given a UL and create a variable
#define KLV_GET_SET_UL(dataset, ul, name) \
    KLVSet name; \
    KLV_GET_SET(dataset, ul, name)

// Convenience macro to decode a nested KLV set given ST and TAG identifiers
#define ST_GET_SET_UL(dataset, ST, TAG, name) \
    KLV_GET_SET_UL(dataset, KLV_ST_TAG(ST, TAG), name)

// Iterate over all children of a dataset
#define KLV_FOR_EACH_CHILD(dataset, child) \
    for (const auto& child : dataset.children())

