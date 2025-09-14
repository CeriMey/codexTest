#pragma once
#include <vector>
#include <cstdint>

class KLVNode {
public:
    virtual ~KLVNode() = default;
    virtual std::vector<uint8_t> encode() const = 0;
    virtual void decode(const std::vector<uint8_t>& data) = 0;
};
