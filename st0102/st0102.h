#pragma once

#include "klv.h"
#include "st_common.h"

namespace misb {
namespace st0102 {

constexpr uint8_t ST_ID = 0x02;
constexpr UL CLASSIFICATION        = make_st_ul(ST_ID, 0x00);
constexpr UL CLASSIFICATION_SYSTEM = make_st_ul(ST_ID, 0x01);

// Register encode/decode lambdas for the above ULs
void register_st0102(KLVRegistry& reg);

} // namespace st0102
} // namespace misb

