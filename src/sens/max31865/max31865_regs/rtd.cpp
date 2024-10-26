#include "sens/max31865/rtd.hpp"

namespace sens {
namespace max31865 {
    RTD::RTD(const std::array<std::bitset<8>, 2>& serialized) :
        adc_code { serialized },
        fault { (serialized[1][0] == true) ? Masks::RTD_LSBs::Fault::Or::FAULT : Masks::RTD_LSBs::Fault::Or::NOFAULT }
    {}

    std::optional<float> RTD::calculate_approx_temp() const {
        if(fault == Masks::RTD_LSBs::Fault::Or::FAULT) {
            return std::nullopt;
        }
        return (static_cast<float>(adc_code.value.unwrap()) / 32.0f) - 256.0f;
    }

    bool RTD::operator==(const RTD& other) const {
        return (
            (adc_code == other.adc_code)
            && (fault == other.fault)
        );
    }

    bool RTD::operator!=(const RTD& other) const {
        return !(*this == other);
    }
}
}