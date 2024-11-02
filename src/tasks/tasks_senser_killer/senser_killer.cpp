#include <trielo/trielo.hpp>
#include "bksram/bksram.hpp"
#include "actu/fan/ctl/ctl.hpp"
#include "actu/fan/fb/fb.hpp"
#include "spi.h"
#include "main.h"
#include "sens/max31865/max31865.hpp"
#include "panel/sevseg/green_yellow/green_yellow.hpp"
#include "tasks/senser_killer.hpp"

namespace tasks {
    SenserKiller& SenserKiller::get_instance() {
        static SenserKiller instance {};
        return instance;
    }

    using namespace sens;
    static const max31865::FaultThreshold fault_threshold {
        max31865::ADC_Code { max31865::RTD(40.0f).adc_code },
        max31865::ADC_Code { max31865::RTD(-20.0f).adc_code }
    };

    static const max31865::Configuration configuration {
        max31865::Masks::Configuration::Vbias::Or::ON,
        max31865::Masks::Configuration::ConversionMode::Or::AUTO,
        max31865::Masks::Configuration::WireMode::Or::TWO_WIRE_OR_FOUR_WIRE,
        max31865::Masks::Configuration::FaultStatusAutoClear::Or::NOCLEAR
    };

    max31865::Transceiver transceiver_front { &hspi3, SPI3_TEMP_NSS0_GPIO_Port, SPI3_TEMP_NSS0_Pin };
    max31865::Transceiver transceiver_rear { &hspi3, SPI3_TEMP_NSS1_GPIO_Port, SPI3_TEMP_NSS1_Pin };
    max31865::Extension extension_front { transceiver_front };
    max31865::Extension extension_rear { transceiver_rear };

    static void init() {
        if(transceiver_front.init() != HAL_OK) {
            bksram::write_reset<0xE'50'01>();
        } else if(transceiver_rear.init() != HAL_OK) {
            bksram::write_reset<0xE'50'02>();
        }

        if(extension_front.configure_clear() != HAL_OK) {
            bksram::write_reset<0xE'50'03>();
        } else if(extension_rear.configure_clear() != HAL_OK) {
            bksram::write_reset<0xE'50'04>();
        }

        if(extension_front.is_clear_configured().value_or(false) == false) {
            bksram::write_reset<0xE'50'05>();
        } else if(extension_rear.is_clear_configured().value_or(false) == false) {
            bksram::write_reset<0xE'50'06>();
        }

        if(extension_front.configure_notch_filter_frequency(max31865::Masks::Configuration::FilterSelect::Or::FIFTY_HZ) != HAL_OK) {
            bksram::write_reset<0xE'50'07>();
        } else if(extension_rear.configure_notch_filter_frequency(max31865::Masks::Configuration::FilterSelect::Or::FIFTY_HZ) != HAL_OK) {
            bksram::write_reset<0xE'50'08>();
        }

        if(extension_front.configure(configuration, fault_threshold) != HAL_OK) {
            bksram::write_reset<0xE'50'09>();
        } else if(extension_rear.configure(configuration, fault_threshold) != HAL_OK) {
            bksram::write_reset<0xE'50'10>();
        }
    }

    void SenserKiller::worker(void* arg) {
        SenserKiller& self { *static_cast<SenserKiller*>(arg) };

        Trielo::trielo<actu::fan::ctl::all::start_min_speed>();
        Trielo::trielo<actu::fan::fb::all::init>();
        panel::sevseg::green_yellow::MAX6549 max6549 {};
        init();

        while(1) {
            const auto rtd_front { extension_front.read_rtd() };
            if(rtd_front.has_value() && rtd_front.value().fault == sens::max31865::Masks::RTD_LSBs::Fault::Or::FAULT) {
                bksram::write_reset<0xE'50'11>();
            }
            osDelay(1);

            const auto rtd_rear { extension_rear.read_rtd() };
            if(rtd_rear.has_value() && rtd_rear.value().fault == sens::max31865::Masks::RTD_LSBs::Fault::Or::FAULT) {
                bksram::write_reset<0xE'50'12>();
            }
            osDelay(1);

            self.temp_front = rtd_front.value().calculate_approx_temp().value();
            max6549.yellow_show(self.temp_front);

            self.temp_rear = rtd_rear.value().calculate_approx_temp().value();

            osDelay(1'000);
        }
    }

    /*
    void SenserKiller::worker(void* arg) {
        SenserKiller& self { *static_cast<SenserKiller*>(arg) };

        if(transceiver_front.init() == false) {
            bksram::write_reset<0xE'50'01>();
        } else if(transceiver_rear.init() == false) {
            bksram::write_reset<0xE'50'02>();
        }

        if(extension_front.configure_clear() != HAL_OK) {
            bksram::write_reset<0xE'50'03>();
        } else if(extension_rear.configure_clear() != HAL_OK) {
            bksram::write_reset<0xE'50'04>();
        }

        if(extension_front.is_clear_configured().value_or(true) == false) {
            bksram::write_reset<0xE'50'05>();
        } else if(extension_rear.is_clear_configured().value_or(true) == false) {
            bksram::write_reset<0xE'50'06>();
        }

        if(extension_front.configure_notch_filter_frequency(max31865::Masks::Configuration::FilterSelect::Or::FIFTY_HZ) != HAL_OK) {
            bksram::write_reset<0xE'50'07>();
        } else if(extension_rear.configure_notch_filter_frequency(max31865::Masks::Configuration::FilterSelect::Or::FIFTY_HZ) != HAL_OK) {
            bksram::write_reset<0xE'50'08>();
        }

        if(extension_front.configure(configuration, fault_threshold) != HAL_OK) {
            bksram::write_reset<0xE'50'09>();
        } else if(extension_rear.configure(configuration, fault_threshold) != HAL_OK) {
            bksram::write_reset<0xE'50'10>();
        }

        while(1) {
            for(size_t i = 0; i < actu::fan::fb::fbs.size(); i++) {
                if(actu::fan::ctl::get_speed(actu::fan::fb::fbs[i].get().fan) < 95) {
                    size_t j = 0;
                    const size_t max_tries { 5 };
                    for(; j < max_tries; j++) {
                        const auto rpm { actu::fan::fb::fbs[i].get().consume_rpm() };
                        if(rpm.has_value() && rpm != 0.0f) {
                            break;
                        } else {
                            osDelay(100);
                        }
                    }
                    if(i == max_tries) {
                        bksram::write_reset<0xE'F0'01>();
                    }
                }
                osDelay(1);
            }

            const auto rtd_front { extension_front.read_rtd() };
            if(rtd_front.has_value() && rtd_front.value().fault == sens::max31865::Masks::RTD_LSBs::Fault::Or::FAULT) {
                bksram::write_reset<0xE'50'11>();
            }
            osDelay(1);

            const auto rtd_rear { extension_rear.read_rtd() };
            if(rtd_rear.has_value() && rtd_rear.value().fault == sens::max31865::Masks::RTD_LSBs::Fault::Or::FAULT) {
                bksram::write_reset<0xE'50'12>();
            }
            osDelay(1);

            self.temp_front = rtd_front.value().calculate_approx_temp().value();
            self.temp_rear = rtd_rear.value().calculate_approx_temp().value();
            osDelay(1);
        }
    }
    */
}