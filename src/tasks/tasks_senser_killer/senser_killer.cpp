#include <trielo/trielo.hpp>
#include "bksram/bksram.hpp"
#include "actu/fan/ctl/ctl.hpp"
#include "actu/fan/fb/fb.hpp"
#include "spi.h"
#include "main.h"
#include "panel/sevseg/green_yellow/green_yellow.hpp"
#include "tasks/senser_killer.hpp"

namespace tasks {
    SenserKiller& SenserKiller::get_instance() {
        static SenserKiller instance {};
        return instance;
    }

    void SenserKiller::init() {
        if(actu::fan::ctl::all::init() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::Init::Fan::CTL_ALL_INIT>();
        }

        actu::fan::ctl::all::start_min_speed();

        if(actu::fan::fb::all::init() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::Init::Fan::FB_ALL_INIT>();
        }

        if(transceiver_front.init() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::TransceiverInit::FRONT>();
        } else if(transceiver_rear.init() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::TransceiverInit::REAR>();
        }

        if(extension_front.configure(sens::max31865::Configuration()) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::ClearConfigure::FRONT>();
        } else if(extension_rear.configure(sens::max31865::Configuration()) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::ClearConfigure::REAR>();
        }

        if(extension_front.read_configuration() != sens::max31865::Configuration()) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::ClearConfigureValidation::FRONT>();
        } else if(extension_rear.read_configuration() != sens::max31865::Configuration()) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::ClearConfigureValidation::REAR>();
        }

        if(extension_front.set_filter_select(sens::max31865::Masks::FilterSelect::Or::FIFTY_HZ) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::SetFilterSelect::FRONT>();
        } else if(extension_rear.set_filter_select(sens::max31865::Masks::FilterSelect::Or::FIFTY_HZ) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::SetFilterSelect::REAR>();
        }

        if(extension_front.read_filter_select() != sens::max31865::Masks::FilterSelect::Or::FIFTY_HZ) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::FilterSelectValidation::FRONT>();
        } else if(extension_rear.read_filter_select() != sens::max31865::Masks::FilterSelect::Or::FIFTY_HZ) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::FilterSelectValidation::REAR>();
        }

        if(extension_front.set_fault_threshold(fault_threshold) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::SetFaultThreshold::FRONT>();
        } else if(extension_rear.set_fault_threshold(fault_threshold) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::SetFaultThreshold::REAR>();
        }

        if(extension_front.read_fault_threshold() != fault_threshold) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::FaultThresholdValidation::FRONT>();
        } else if(extension_rear.read_fault_threshold() != fault_threshold) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::FaultThresholdValidation::REAR>();
        }

        if(extension_front.configure(configuration) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::Configure::FRONT>();
        } else if(extension_rear.configure(configuration) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::Configure::REAR>();
        }

        if(extension_front.read_configuration() != configuration) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::ConfigureValidation::FRONT>();
        } else if(extension_rear.read_configuration() != configuration) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::ConfigureValidation::REAR>();
        }

        if(extension_front.clear_fault_status() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::ClearFaultStatus::FRONT>();
        } else if(extension_rear.clear_fault_status() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::ClearFaultStatus::REAR>();
        }
        
        const auto fault_status_nofault { sens::max31865::FaultStatus(std::bitset<8>(0b0000'0000)) };
        if(extension_front.read_fault_status() != fault_status_nofault) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::ClearFaultStatusValidation::FRONT>();
        } else if(extension_rear.read_fault_status() != fault_status_nofault) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::ClearFaultStatusValidation::REAR>();
        }

        if(extension_front.run_auto_fault_detection() != fault_status_nofault) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::RunAutoFaultDetection::FRONT>();
        } else if(extension_rear.run_auto_fault_detection() != fault_status_nofault) {
            bksram::write_reset<bksram::ErrorCodes::Init::MAX31865::Extension::RunAutoFaultDetection::REAR>();
        }

        for(const auto& e: actu::fan::fb::fbs) {
            if(e.get().consume_rpm() != 0.0f) {
                bksram::write_reset<bksram::ErrorCodes::Init::Fan::FB_ALL_TEST>();
            }
        }
    }

    void SenserKiller::worker(void* arg) {
        SenserKiller& self { *static_cast<SenserKiller*>(arg) };

        while(1) {
            const auto rtd_front { self.extension_front.read_rtd() };
            if(rtd_front.has_value()) {
                bksram::write_reset<bksram::ErrorCodes::Running::MAX31865::RTD::Timeout::FRONT>();
            }
            const auto rtd_rear { self.extension_rear.read_rtd() };
            if(rtd_rear.has_value()) {
                bksram::write_reset<bksram::ErrorCodes::Running::MAX31865::RTD::Timeout::REAR>();
            }

            if(rtd_front.value().fault == sens::max31865::Masks::RTD_LSBs::Fault::Or::FAULT) {
                bksram::write_reset<bksram::ErrorCodes::Running::MAX31865::RTD::HighOrLowFaultThreshold::FRONT>();
            } else if(rtd_rear.value().fault == sens::max31865::Masks::RTD_LSBs::Fault::Or::FAULT) {
                bksram::write_reset<bksram::ErrorCodes::Running::MAX31865::RTD::HighOrLowFaultThreshold::REAR>();
            }

            self.temp_front = rtd_front.value().calculate_approx_temp().value();
            self.max6549.yellow_show(self.temp_front);

            self.temp_rear = rtd_rear.value().calculate_approx_temp().value();
            self.max6549.green_show(self.temp_rear);

            osDelay(1);
        }
    }
}