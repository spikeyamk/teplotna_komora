#include <iostream>

#include "sens/max31865/adc_code.hpp"
#include "sens/max31865/configuration.hpp"
#include "sens/max31865/fault_status.hpp"
#include "sens/max31865/fault_threshold.hpp"
#include "sens/max31865/rtd.hpp"
#include "sens/max31865/test.hpp"

namespace sens {
namespace max31865 {
namespace test {
    const ADC_Code::uint15_t number { 0b0001'0011'0000'1111 };
    const std::array<std::bitset<8>, 2> number_serialized {{
        { 0b0010'0110 },
        { 0b0001'1110 },
    }};
    const ADC_Code adc_code_value { number };

    int adc_code() {
        if(adc_code_value.serialize() != number_serialized) {
            for(size_t i = 0; i < adc_code_value.serialize().size(); i++) {
                std::cout
                    << "adc_code_value.serialize()["
                    << i
                    << "]: "
                    << adc_code_value.serialize()[i]
                    << std::endl;
                std::cout
                    << "number_serialized["
                    << i
                    << "]: "
                    << number_serialized[i]
                    << std::endl;
            }
            return -1;
        }

        if(ADC_Code(adc_code_value.serialize()) != adc_code_value) {
            std::cout
                << "ADC_Code(adc_code_value.serialize()).value: "
                << ADC_Code(adc_code_value.serialize()).value
                << std::endl;
            std::cout
                << "adc_code_value.value: "
                << adc_code_value.value
                << std::endl;
            return -2;
        }

        return 0;
    }

    int configuration() {
        return -1;
    }

    int fault_status() {
        return -1;
    }

    int fault_threshold() {
        return -1;
    }
    
    const RTD rtd_nofault { number_serialized };

    const std::array<std::bitset<8>, 2> rtd_fault_serialized {
        [&]() {
            auto ret { number_serialized };
            ret[1][0] = true;
            return ret;
        }()
    };

    const RTD rtd_fault {
        [&]() {
            RTD ret { number_serialized };
            ret.adc_code = adc_code_value;
            ret.fault = Masks::RTD_LSBs::Fault::Or::FAULT;
            return ret;
        }()
    };

    int rtd() {
        if(rtd_nofault.fault != Masks::RTD_LSBs::Fault::Or::NOFAULT) {
            return -1;
        }

        if(rtd_nofault.adc_code != adc_code_value) {
            return -2;
        }

        if(RTD(number_serialized) == rtd_fault) {
            return -3;
        }

        if(RTD(number_serialized).fault != Masks::RTD_LSBs::Fault::Or::NOFAULT) {
            return -4;
        }

        if(RTD(number_serialized).adc_code != adc_code_value) {
            return -5;
        }

        if(RTD(rtd_fault_serialized).fault != Masks::RTD_LSBs::Fault::Or::FAULT) {
            return -6;
        }

        if(RTD(rtd_fault_serialized).adc_code != adc_code_value) {
            return -7;
        }

        return 0;
    }
}
}
}