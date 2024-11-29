#include <trielo/trielo.hpp>
#include "bksram/bksram.hpp"
#include "tasks/panel.hpp"
#include "tasks/temp_ctl.hpp"
#include "tasks/senser_killer.hpp"

namespace tasks {
    SenserKiller& SenserKiller::get_instance() {
        static SenserKiller instance {};
        return instance;
    }

    void SenserKiller::init_max31865() {
        if(max31865_transceiver_front.init() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::TransceiverInit::FRONT>();
        } else if(max31865_transceiver_rear.init() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::TransceiverInit::REAR>();
        }

        const char extension_front_name[] { "temp_front" };
        const char extension_rear_name[] { "temp_rear" };
        if(max31865_extension_front.init(extension_front_name) == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::Init::FRONT>();
        } else if(max31865_extension_rear.init(extension_rear_name) == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::Init::REAR>();
        }

        if(max31865_extension_front.configure(sens::max31865::Configuration()) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ClearConfigure::FRONT>();
        } else if(max31865_extension_rear.configure(sens::max31865::Configuration()) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ClearConfigure::REAR>();
        }

        if(max31865_extension_front.read_configuration() != sens::max31865::Configuration()) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ClearConfigureValidation::FRONT>();
        } else if(max31865_extension_rear.read_configuration() != sens::max31865::Configuration()) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ClearConfigureValidation::REAR>();
        }

        if(max31865_extension_front.set_filter_select(sens::max31865::Masks::FilterSelect::Or::FIFTY_HZ) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::SetFilterSelect::FRONT>();
        } else if(max31865_extension_rear.set_filter_select(sens::max31865::Masks::FilterSelect::Or::FIFTY_HZ) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::SetFilterSelect::REAR>();
        }

        if(max31865_extension_front.read_filter_select() != sens::max31865::Masks::FilterSelect::Or::FIFTY_HZ) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::FilterSelectValidation::FRONT>();
        } else if(max31865_extension_rear.read_filter_select() != sens::max31865::Masks::FilterSelect::Or::FIFTY_HZ) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::FilterSelectValidation::REAR>();
        }

        if(max31865_extension_front.set_fault_threshold(fault_threshold) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::SetFaultThreshold::FRONT>();
        } else if(max31865_extension_rear.set_fault_threshold(fault_threshold) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::SetFaultThreshold::REAR>();
        }

        if(max31865_extension_front.read_fault_threshold() != fault_threshold) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::FaultThresholdValidation::FRONT>();
        } else if(max31865_extension_rear.read_fault_threshold() != fault_threshold) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::FaultThresholdValidation::REAR>();
        }

        if(max31865_extension_front.configure(configuration) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::Configure::FRONT>();
        } else if(max31865_extension_rear.configure(configuration) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::Configure::REAR>();
        }

        if(max31865_extension_front.read_configuration() != configuration) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ConfigureValidation::FRONT>();
        } else if(max31865_extension_rear.read_configuration() != configuration) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ConfigureValidation::REAR>();
        }

        if(max31865_extension_front.clear_fault_status() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ClearFaultStatus::FRONT>();
        } else if(max31865_extension_rear.clear_fault_status() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ClearFaultStatus::REAR>();
        }
        
        const auto fault_status_nofault { sens::max31865::FaultStatus(std::bitset<8>(0b0000'0000)) };
        if(max31865_extension_front.read_fault_status() != fault_status_nofault) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ClearFaultStatusValidation::FRONT>();
        } else if(max31865_extension_rear.read_fault_status() != fault_status_nofault) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::ClearFaultStatusValidation::REAR>();
        }

        if(max31865_extension_front.run_auto_fault_detection() != fault_status_nofault) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::RunAutoFaultDetection::FRONT>();
        } else if(max31865_extension_rear.run_auto_fault_detection() != fault_status_nofault) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::RunAutoFaultDetection::REAR>();
        }

        const auto rtd_no_sem_front { max31865_extension_front.read_rtd_no_sem() };
        if(rtd_no_sem_front.has_value() == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::RTD::NoSem::FRONT>();
        }
        const auto rtd_no_sem_rear { max31865_extension_rear.read_rtd_no_sem() };
        if(rtd_no_sem_rear.has_value() == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::RTD::NoSem::REAR>();
        }

        if(rtd_no_sem_front.value().calculate_approx_temp().has_value() == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::RTD::NoSemFault::FRONT>();
        }
        if(rtd_no_sem_rear.value().calculate_approx_temp().has_value() == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::RTD::NoSemFault::REAR>();
        }

        const auto rtd_front { max31865_extension_front.read_rtd() };
        if(rtd_front.has_value() == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::RTD::Sem::FRONT>();
        }
        const auto rtd_rear { max31865_extension_rear.read_rtd() };
        if(rtd_rear.has_value() == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::RTD::Sem::REAR>();
        }

        if(rtd_front.value().calculate_approx_temp().has_value() == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::RTD::SemFault::FRONT>();
        }
        if(rtd_rear.value().calculate_approx_temp().has_value() == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::MAX31865::Extension::RTD::SemFault::REAR>();
        }

        Panel::get_instance().push(
            Panel::Menu::Events::MAX31865_Sample(
                rtd_front.value(),
                rtd_rear.value()
            ),
            Panel::Timeout::NotIRQ
        );
    }

    void SenserKiller::init_sht31() {
        const auto sht31_extension_inside_init { sht31_extension_inside.init() };
        if(sht31_extension_inside_init.has_value() == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::SHT31::Init::HAL::INSIDE>();
        }
        const auto sht31_extension_outside_init { sht31_extension_inside.init() };
        if(sht31_extension_outside_init.has_value() == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::SHT31::Init::HAL::OUTSIDE>();
        }

        if(sht31_extension_inside_init.value() != true) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::SHT31::Init::Crc::INSIDE>();
        }
        if(sht31_extension_outside_init.value() != true) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::SHT31::Init::Crc::OUTSIDE>();
        }
        
        if(sht31_extension_inside.disable_heater() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::SHT31::Init::DisableHeater::INSIDE>();
        }
        if(sht31_extension_outside.disable_heater() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::SHT31::Init::DisableHeater::OUTSIDE>();
        }

        const auto read_status_inside { sht31_extension_inside.read_status() };
        if(read_status_inside.has_value() == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::SHT31::Init::ReadStatus::INSIDE>();
        }
        const auto read_status_outside { sht31_extension_outside.read_status() };
        if(read_status_inside.has_value() == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::SHT31::Init::ReadStatus::OUTSIDE>();
        }

        if(read_status_inside.value().heater != sens::sht31::Masks::Status::Heater::Or::OFF) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::SHT31::Init::StatusHeater::INSIDE>();
        }
        if(read_status_outside.value().heater != sens::sht31::Masks::Status::Heater::Or::OFF) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::SHT31::Init::StatusHeater::OUTSIDE>();
        }
        
        if(sht31_extension_inside.start_periodic_mode_ten_hertz(sens::sht31::Commands::Periodic::TEN_HERTZ::LOW_REPEATABILITY) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::SHT31::Init::Start::INSIDE>();
        }
        if(sht31_extension_outside.start_periodic_mode_ten_hertz(sens::sht31::Commands::Periodic::TEN_HERTZ::LOW_REPEATABILITY) != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::SHT31::Init::Start::OUTSIDE>();
        }

        osDelay(110);
        
        const auto ret_temp_hum_inside { sht31_extension_inside.read_temp_hum_periodic_mode() };
        if(ret_temp_hum_inside.has_value() == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::SHT31::TempHum::INSIDE>();
        }
        const auto ret_temp_hum_outside { sht31_extension_outside.read_temp_hum_periodic_mode() };
        if(ret_temp_hum_outside.has_value() == false) {
            bksram::write_reset<bksram::ErrorCodes::SenserKiller::Init::SHT31::TempHum::OUTSIDE>();
        }

        sht31_inside = ret_temp_hum_inside.value();
        sht31_outside = ret_temp_hum_outside.value();

        Panel::Menu::Actions::save_sample<
            Panel::Menu::States::SHT31_Inside,
            Panel::Menu::Events::SHT31_InsideSample
        >(
            Panel::get_instance().sht31_inside,
            Panel::Menu::Events::SHT31_InsideSample(
                ret_temp_hum_inside.value()
            )
        );

        Panel::Menu::Actions::save_sample<
            Panel::Menu::States::SHT31_Outside,
            Panel::Menu::Events::SHT31_OutsideSample
        >(
            Panel::get_instance().sht31_outside,
            Panel::Menu::Events::SHT31_OutsideSample(
                ret_temp_hum_outside.value()
            )
        );
    }

    void SenserKiller::init() {
        init_sht31();
        init_max31865();
    }

    void SenserKiller::worker(void* arg) {
        SenserKiller& self { *static_cast<SenserKiller*>(arg) };
        self.init();

        for(uint8_t sht31 = 0, max31865 = 0; true; max31865++) {
            const auto rtd_front { self.max31865_extension_front.read_rtd() };
            if(rtd_front.has_value() == false) {
                bksram::write_reset<bksram::ErrorCodes::SenserKiller::Worker::MAX31865::RTD::Timeout::FRONT>();
            }
            const auto rtd_rear { self.max31865_extension_rear.read_rtd() };
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

            if((max31865 % 10) == 0) {
                const auto sht31_inside { self.sht31_extension_inside.read_temp_hum_periodic_mode() };
                if(sht31_inside.has_value() == false) {
                    bksram::write_reset<bksram::ErrorCodes::SenserKiller::Worker::SHT31::TempHum::INSIDE>();
                }

                const auto sht31_outside { self.sht31_extension_outside.read_temp_hum_periodic_mode() };
                if(sht31_outside.has_value() == false) {
                    bksram::write_reset<bksram::ErrorCodes::SenserKiller::Worker::SHT31::TempHum::OUTSIDE>();
                }

                self.sht31_inside  = sht31_inside.value();
                self.sht31_outside = sht31_outside.value();

                TempCtl::get_instance().push(
                    TempCtl::Controller::Events::SHT31_Sample {
                        .inside = sht31_inside.value(),
                        .outside = sht31_outside.value(),
                    }
                );

                sht31++;
                if(sht31 == 5) {
                    sht31 = 0;

                    Panel::get_instance().push(
                        Panel::Menu::Events::SHT31_InsideSample(sht31_inside.value()),
                        Panel::Timeout::NotIRQ
                    );
                    Panel::get_instance().push(
                        Panel::Menu::Events::SHT31_OutsideSample(sht31_outside.value()),
                        Panel::Timeout::NotIRQ
                    );
                }
            }

            TempCtl::get_instance().push(
                TempCtl::Controller::Events::MAX31865_Sample {
                    .front = rtd_front.value(),
                    .rear = rtd_rear.value(),
                }
            );

            if(max31865 == 50) {
                max31865 = 0;

                Panel::get_instance().push(
                    Panel::Menu::Events::MAX31865_Sample (
                        rtd_front.value(),
                        rtd_rear.value()
                    ),
                    Panel::Timeout::NotIRQ
                );
            }
        }
    }

    osStatus SenserKiller::release_semaphore_front() {
        return max31865_extension_front.release_semaphore();
    }

    osStatus SenserKiller::release_semaphore_rear() {
        return max31865_extension_rear.release_semaphore();
    }
}