#pragma once
#include "klv_node.h"
#include "klv_types.h"
#include <vector>

class KLVBytes : public KLVNode {
public:
    KLVBytes(const UL& ul, const std::vector<uint8_t>& value = {});
    std::vector<uint8_t> encode() const override;
    void decode(const std::vector<uint8_t>& data) override;
    const std::vector<uint8_t>& value() const { return value_; }
    void set_value(const std::vector<uint8_t>& v) { value_ = v; }
    const UL& ul() const { return ul_; }
private:
    UL ul_;
    std::vector<uint8_t> value_;
};
