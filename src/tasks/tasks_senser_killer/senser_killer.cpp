#include <trielo/trielo.hpp>
#include "bksram/bksram.hpp"
#include "spi.h"
#include "main.h"
#include "tasks/senser_killer.hpp"

namespace tasks {
    SenserKiller& SenserKiller::get_instance() {
        static SenserKiller instance {};
        return instance;
    }

    void SenserKiller::init() {
        if(transceiver_front.init() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::TransceiverInit::FRONT>();
        } else if(transceiver_rear.init() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::TransceiverInit::REAR>();
        }

        const char extension_front_name[] { "temp_front" };
        const char extension_rear_name[] { "temp_rear" };
        if(extension_front.init(extension_front_name) == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::Init::FRONT>();
        } else if(extension_rear.init(extension_rear_name) == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::Init::REAR>();
        }

        if(extension_front.configure(sens::max31865::Configuration()) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ClearConfigure::FRONT>();
        } else if(extension_rear.configure(sens::max31865::Configuration()) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ClearConfigure::REAR>();
        }

        if(extension_front.read_configuration() != sens::max31865::Configuration()) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ClearConfigureValidation::FRONT>();
        } else if(extension_rear.read_configuration() != sens::max31865::Configuration()) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ClearConfigureValidation::REAR>();
        }

        if(extension_front.set_filter_select(sens::max31865::Masks::FilterSelect::Or::FIFTY_HZ) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::SetFilterSelect::FRONT>();
        } else if(extension_rear.set_filter_select(sens::max31865::Masks::FilterSelect::Or::FIFTY_HZ) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::SetFilterSelect::REAR>();
        }

        if(extension_front.read_filter_select() != sens::max31865::Masks::FilterSelect::Or::FIFTY_HZ) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::FilterSelectValidation::FRONT>();
        } else if(extension_rear.read_filter_select() != sens::max31865::Masks::FilterSelect::Or::FIFTY_HZ) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::FilterSelectValidation::REAR>();
        }

        if(extension_front.set_fault_threshold(fault_threshold) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::SetFaultThreshold::FRONT>();
        } else if(extension_rear.set_fault_threshold(fault_threshold) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::SetFaultThreshold::REAR>();
        }

        if(extension_front.read_fault_threshold() != fault_threshold) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::FaultThresholdValidation::FRONT>();
        } else if(extension_rear.read_fault_threshold() != fault_threshold) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::FaultThresholdValidation::REAR>();
        }

        if(extension_front.configure(configuration) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::Configure::FRONT>();
        } else if(extension_rear.configure(configuration) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::Configure::REAR>();
        }

        if(extension_front.read_configuration() != configuration) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ConfigureValidation::FRONT>();
        } else if(extension_rear.read_configuration() != configuration) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ConfigureValidation::REAR>();
        }

        if(extension_front.clear_fault_status() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ClearFaultStatus::FRONT>();
        } else if(extension_rear.clear_fault_status() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ClearFaultStatus::REAR>();
        }
        
        const auto fault_status_nofault { sens::max31865::FaultStatus(std::bitset<8>(0b0000'0000)) };
        if(extension_front.read_fault_status() != fault_status_nofault) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ClearFaultStatusValidation::FRONT>();
        } else if(extension_rear.read_fault_status() != fault_status_nofault) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ClearFaultStatusValidation::REAR>();
        }

        if(extension_front.run_auto_fault_detection() != fault_status_nofault) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::RunAutoFaultDetection::FRONT>();
        } else if(extension_rear.run_auto_fault_detection() != fault_status_nofault) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::RunAutoFaultDetection::REAR>();
        }

        osDelay(1'000);
    }

    void SenserKiller::worker(void* arg) {
        SenserKiller& self { *static_cast<SenserKiller*>(arg) };
        self.init();

        while(1) {
            const auto rtd_front { self.extension_front.read_rtd() };
            if(rtd_front.has_value() == false) {
                bksram::write_reset<bksram::ErrorCodes::SenserKiller::Worker::MAX31865::RTD::Timeout::FRONT>();
            }
            const auto rtd_rear { self.extension_rear.read_rtd() };
            if(rtd_rear.has_value() == false) {
                bksram::write_reset<bksram::ErrorCodes::SenserKiller::Worker::MAX31865::RTD::Timeout::REAR>();
            }

            if(rtd_front.value().fault == sens::max31865::Masks::RTD_LSBs::Fault::Or::FAULT) {
                bksram::write_reset<bksram::ErrorCodes::SenserKiller::Worker::MAX31865::RTD::HighOrLowFaultThreshold::FRONT>();
            } else if(rtd_rear.value().fault == sens::max31865::Masks::RTD_LSBs::Fault::Or::FAULT) {
                bksram::write_reset<bksram::ErrorCodes::SenserKiller::Worker::MAX31865::RTD::HighOrLowFaultThreshold::REAR>();
            }

            self.rtd_front = rtd_front.value();
            self.rtd_rear = rtd_rear.value();

            osDelay(1);
        }
    }

    osStatus SenserKiller::release_semaphore_front() {
        return extension_front.release_semaphore();
    }

    osStatus SenserKiller::release_semaphore_rear() {
        return extension_rear.release_semaphore();
    }
}