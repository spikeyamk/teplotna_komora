#pragma once

#include <bitset>
#include <array>
#include <bitint.hpp>

#include "sens/max31865/masks.hpp"

namespace sens {
namespace max31865 {
    class ADC_Code {
    public:
        using uint15_t = bitint::ubitint<15>;
        uint15_t value { 0 };
    public:
        ADC_Code() = default;

        constexpr ADC_Code(const uint15_t value) :
            value { value }
        {}

        constexpr ADC_Code(const std::array<std::bitset<8>, 2>& serialized) :
            value {
                static_cast<uint16_t>(
                    (
                        (std::bitset<16>(serialized[0].to_ulong()) << 7)
                        | (std::bitset<16>((serialized[1] & Masks::RTD_LSBs::Data::AND).to_ulong()) >> 1)
                    ).to_ulong()
                )
            }
        {}

        constexpr std::array<std::bitset<8>, 2> serialize() const {
            const std::bitset<16> unwrapped_shifted_value {
            std::bitset<16>(value.unwrap()) << 1
            };
            const std::bitset<8> msb {
                (
                    (unwrapped_shifted_value >> 8)
                    & std::bitset<16>(0xFF)
                ).to_ulong()
            };
            const std::bitset<8> lsb {
                (
                    unwrapped_shifted_value
                    & std::bitset<16>(0xFF)
                ).to_ulong()
            };
            return { msb, lsb };
        }
    public:
        bool operator==(const ADC_Code& other) const;
        bool operator!=(const ADC_Code& other) const;
    };
}
}