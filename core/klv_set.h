#pragma once
#include "klv_node.h"
#include <memory>
#include <vector>

class KLVSet : public KLVNode {
public:
    KLVSet(bool use_ul_keys = true, uint8_t st_id = 0);
    void add(std::shared_ptr<KLVNode> node);
    std::vector<uint8_t> encode() const override;
    void decode(const std::vector<uint8_t>& data) override;
    const std::vector<std::shared_ptr<KLVNode>>& children() const { return children_; }
    bool uses_ul_keys() const { return use_ul_keys_; }
private:
    std::vector<std::shared_ptr<KLVNode>> children_;
    bool use_ul_keys_;
    uint8_t st_id_;
};
