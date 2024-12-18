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
        const Configuration configuration {
            Masks::Configuration::Vbias::Or::ON,
            Masks::Configuration::ConversionMode::Or::AUTO,
            Masks::Configuration::WireMode::Or::TWO_WIRE_OR_FOUR_WIRE
        };

        if(Configuration(configuration.serialize()) != configuration) {
            return 1;
        }

        return 0;
    }

    int fault_status() {
        const std::bitset<8> fault_status_serialized {
            0b1000'1100
        };

        const FaultStatus fault_status {
            Masks::FaultStatus::AlwaysActive::Or::OVERVOLTAGE_OR_UNDERVOLTAGE,
            Masks::FaultStatus::MasterInitiated::REFIN_MinusIsGreaterThanZeroPointEightFiveTimesVbias::Or::NOFAULT,
            Masks::FaultStatus::MasterInitiated::REFIN_MinusIsLowerThanZeroPointEightFiveTimesVbiasForceMinusOpen::Or::NOFAULT,
            Masks::FaultStatus::MasterInitiated::RTDIN_MinusIsLowerThanZeroPointEightFiveTimesVbiasForceMinusOpen::Or::FAULT,
            Masks::FaultStatus::EveryConversion::Or::RTD_OPEN,
        };

        if(FaultStatus(fault_status_serialized) != fault_status) {
            return 1;
        }

        return 0;
    }

    const auto another_number { number - 10 };
    const std::array<std::bitset<8>, 2> another_number_serialized {{
        { 0b0010'0110 },
        { 0b0000'1010 },
    }};

    const FaultThreshold fault_threshold_value {
        adc_code_value,
        { another_number },
    };

    const std::array<std::bitset<8>, 4> fault_threshold_serialized {
        number_serialized[0],
        number_serialized[1],
        another_number_serialized[0],
        another_number_serialized[1],
    };

    int fault_threshold() {
        if(fault_threshold_value != FaultThreshold(fault_threshold_serialized)) {
            return 1;
        }

        if(fault_threshold_value != FaultThreshold(fault_threshold_value.serialize())) {
            return 2;
        }

        return 0;
    }
    
    const RTD rtd_nofault { number_serialized };

    const std::array<std::bitset<8>, 2> rtd_fault_serialized {
        [](const std::array<std::bitset<8>, 2>& number_serialized) {
            auto ret { number_serialized };
            ret[1][0] = true;
            return ret;
        }(number_serialized)
    };

    const RTD rtd_fault {
        [](const std::array<std::bitset<8>, 2>& number_serialized) {
            RTD ret { number_serialized };
            ret.adc_code = adc_code_value;
            ret.fault = Masks::RTD_LSBs::Fault::Or::FAULT;
            return ret;
        }(number_serialized)
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