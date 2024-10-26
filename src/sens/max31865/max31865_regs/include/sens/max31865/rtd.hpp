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
        RTD(const std::array<std::bitset<8>, 2>& serialized);
        std::optional<float> calculate_approx_temp() const;
        bool operator==(const RTD& other) const;
        bool operator!=(const RTD& other) const;
    };
}
}