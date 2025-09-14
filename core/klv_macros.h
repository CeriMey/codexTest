#pragma once

#include "klv.h"
#include "stanag.h"
#include <memory>

// Basic tag and set construction
#define KLV_TAG(tag, value) stanag::TagValue{tag, value}
#define KLV_SET(...) stanag::create_dataset({__VA_ARGS__})
#define KLV_LOCAL_DATASET(...) KLV_SET(__VA_ARGS__)

// Helpers to reference MISB namespaces generically
#define KLV_ST_TAG(ST, TAG) misb::st##ST::TAG
#define KLV_ST_ITEM(ST, TAG, VALUE) KLV_TAG(KLV_ST_TAG(ST, TAG), VALUE)

// Compose STANAG packets using nested macros
#define ST_PACKET(...) KLV_SET(__VA_ARGS__)
#define STANAG4609_PACKET(...) ST_PACKET(__VA_ARGS__)

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

