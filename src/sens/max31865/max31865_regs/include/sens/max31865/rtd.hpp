#pragma once

#include <optional>

#include "sens/max31865/masks.hpp"
#include "sens/max31865/adc_code.hpp"

namespace sens {
namespace max31865 {
    class RTD {
    public:
        ADC_Code adc_code;
        Masks::RTD_LSBs::Fault::Or fault;
    public:
        RTD() = delete;
        constexpr RTD(const float approx_temp) :
            adc_code {
                static_cast<uint16_t>(
                    (static_cast<int>(approx_temp) + 256) * 32
                )
            }
        {}
        RTD(const std::array<std::bitset<8>, 2>& serialized);
        std::optional<float> calculate_approx_temp() const;
        bool operator==(const RTD& other) const;
        bool operator!=(const RTD& other) const;
    };
}
}