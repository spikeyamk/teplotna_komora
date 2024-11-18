#include <bitset>
#include <array>
#include <bitint.hpp>

#include "sens/max31865/adc_code.hpp"

namespace sens {
namespace max31865 {
    bool ADC_Code::operator==(const ADC_Code& other) const {
        return other.value == value;
    }

    bool ADC_Code::operator!=(const ADC_Code& other) const {
        return !(*this == other);
    }
}
}