#pragma once

#include "klv.h"
#include "stanag.h"
#include <memory>

// Basic tag and set construction
#define KLV_TAG(tag, value) stanag::TagValue(tag, value)
#define KLV_SET(...) stanag::create_dataset({__VA_ARGS__})
#define KLV_LOCAL_DATASET(...) stanag::create_dataset({__VA_ARGS__}, false, true)
#define KLV_DATASET(tag, ...) KLV_TAG(tag, KLV_SET(__VA_ARGS__))

// ST helper to embed nested datasets
#define KLV_ST_DATASET(ST, TAG, ...) \
    KLV_DATASET(KLV_ST_TAG(ST, TAG), __VA_ARGS__)

// Helpers to reference MISB namespaces generically
#define KLV_ST_TAG(ST, TAG) misb::st##ST::TAG
#define KLV_ST_ITEM(ST, TAG, VALUE) KLV_TAG(KLV_ST_TAG(ST, TAG), VALUE)

// Compose a complete STANAG 4609 packet from tag/value pairs
#define STANAG4609_PACKET(...) \
    stanag::create_stanag4609_packet({__VA_ARGS__})

// Dataset manipulation helpers
#define KLV_ADD_LEAF(dataset, tag, value) \
    dataset.add(std::make_shared<KLVLeaf>(tag, value))
#define KLV_ADD_BYTES(dataset, tag, bytes) \
    dataset.add(std::make_shared<KLVBytes>(tag, bytes))

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

