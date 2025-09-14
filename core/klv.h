#pragma once
#include <array>
#include <vector>
#include <memory>
#include <map>
#include <functional>

using UL = std::array<uint8_t,16>;

class KLVNode {
public:
    virtual ~KLVNode() = default;
    virtual std::vector<uint8_t> encode() const = 0;
    virtual void decode(const std::vector<uint8_t>& data) = 0;
};

class KLVLeaf : public KLVNode {
public:
    KLVLeaf(const UL& ul, double value = 0.0);
    std::vector<uint8_t> encode() const override;
    void decode(const std::vector<uint8_t>& data) override;
    double value() const { return value_; }
    void set_value(double v) { value_ = v; }
    const UL& ul() const { return ul_; }
private:
    UL ul_;
    double value_;
};

// Leaf node holding raw byte payload (e.g. nested local sets)
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

class KLVSet : public KLVNode {
public:
    void add(std::shared_ptr<KLVNode> node);
    std::vector<uint8_t> encode() const override;
    void decode(const std::vector<uint8_t>& data) override;
private:
    std::vector<std::shared_ptr<KLVNode>> children_;
};

struct KLVEntry {
    std::function<std::vector<uint8_t>(double)> encoder;
    std::function<double(const std::vector<uint8_t>&)> decoder;
};

class KLVRegistry {
public:
    void register_ul(const UL& ul, const KLVEntry& entry);
    const KLVEntry* find(const UL& ul) const;
    static KLVRegistry& instance();
private:
    std::map<UL, KLVEntry> entries_;
};

