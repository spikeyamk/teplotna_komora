#pragma once

#include <bitset>
#include <array>
#include <ubitint.hpp>

namespace sens {
namespace max31865 {
    class ADC_Code {
    public:
        using uint15_t = ubitint_t<15>;
        uint15_t value { 0 };
    public:
        ADC_Code() = default;

        ADC_Code(const uint15_t value) :
            value { value }
        {}

        ADC_Code(const std::array<std::bitset<8>, 2>& serialized);
    public:
        std::array<std::bitset<8>, 2> serialize() const;
        bool operator==(const ADC_Code& other) const;
        bool operator!=(const ADC_Code& other) const;
    };
}
}