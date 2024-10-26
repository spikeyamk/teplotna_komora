#include <bitset>
#include <array>
#include <ubitint.hpp>

#include "sens/max31865/masks.hpp"
#include "sens/max31865/adc_code.hpp"

namespace sens {
namespace max31865 {
    ADC_Code::ADC_Code(const std::array<std::bitset<8>, 2>& serialized) :
        value {
            static_cast<uint16_t>(
                (
                    (std::bitset<16>(serialized[0].to_ulong()) << 7)
                    | (std::bitset<16>((serialized[1] & Masks::RTD_LSBs::Data::AND).to_ulong()) >> 1)
                ).to_ulong()
            )
        }
    {}

    std::array<std::bitset<8>, 2> ADC_Code::serialize() const {
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

    bool ADC_Code::operator==(const ADC_Code& other) const {
        return other.value == value;
    }

    bool ADC_Code::operator!=(const ADC_Code& other) const {
        return !(*this == other);
    }
}
}