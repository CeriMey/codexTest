#pragma once

#include "klv.h"
#include "stanag.h"
#include <memory>

// Macro to create a stanag::TagValue
#define KLV_TAG(tag, value) stanag::TagValue{tag, value}

// Macro to create a local KLV dataset from TagValue entries
#define KLV_LOCAL_DATASET(...) stanag::create_dataset({__VA_ARGS__})

// Macro to add a numeric leaf to an existing dataset
#define KLV_ADD_LEAF(dataset, tag, value) \
    dataset.add(std::make_shared<KLVLeaf>(tag, value))

// Macro to add raw bytes to an existing dataset
#define KLV_ADD_BYTES(dataset, tag, bytes) \
    dataset.add(std::make_shared<KLVBytes>(tag, bytes))
