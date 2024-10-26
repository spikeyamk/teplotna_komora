#pragma once

#include "sens/max31865/adc_code.hpp"

namespace sens {
namespace max31865 {
    class FaultThreshold {
    public:
        ADC_Code high_fault_threshold { ADC_Code::uint15_t { 0x7F'FF } };
        ADC_Code low_fault_threshold  { ADC_Code::uint15_t { 0x00'00 } };
    public:
        FaultThreshold() = default;
        FaultThreshold(const ADC_Code high_fault_threshold, const ADC_Code low_fault_threshold);
        FaultThreshold(const std::array<std::bitset<8>, 4>& serialized);
    public:
        std::array<std::bitset<8>, 4> serialize() const;
        bool operator==(const FaultThreshold& other) const;
        bool operator!=(const FaultThreshold& other) const;
    };
}
}