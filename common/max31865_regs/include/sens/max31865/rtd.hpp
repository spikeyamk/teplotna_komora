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
                    (approx_temp + 256.0f) * 32.0f
                )
            },
            fault { Masks::RTD_LSBs::Fault::Or::NOFAULT }
        {}


        constexpr RTD(const std::array<std::bitset<8>, 2>& serialized) :
            adc_code { serialized },
            fault { (serialized[1][0] == true) ? Masks::RTD_LSBs::Fault::Or::FAULT : Masks::RTD_LSBs::Fault::Or::NOFAULT }
        {}

        std::optional<float> calculate_approx_temp() const;
        bool operator==(const RTD& other) const;
        bool operator!=(const RTD& other) const;
    };

    static constexpr uint16_t blablabla { RTD(50.0f).adc_code.value.unwrap() };
}
}