#pragma once
#include "klv_node.h"
#include "klv_types.h"
#include <vector>

class KLVLeaf : public KLVNode {
public:
    KLVLeaf(const UL& ul, double value = 0.0, bool use_tag = false);
    std::vector<uint8_t> encode() const override;
    void decode(const std::vector<uint8_t>& data) override;
    double value() const { return value_; }
    void set_value(double v) { value_ = v; }
    const UL& ul() const { return ul_; }
private:
    UL ul_;
    double value_;
    bool use_tag_;
};
