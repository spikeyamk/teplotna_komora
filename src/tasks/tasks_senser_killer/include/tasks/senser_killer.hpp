#pragma once

#include <array>
#include <iostream>
#include <tuple>

#include "main.h"
#include "spi.h"
#include "i2c.h"
#include "sens/max31865/max31865.hpp"
#include "sens/sht31/sht31.hpp"
#include "tasks/task.hpp"

namespace tasks {
    class SenserKiller : public Task<SenserKiller, 4 * 1024, "senser_killer"> {
        friend CRTP;
    public:
        sens::max31865::RTD rtd_front { 0.0f };
        sens::max31865::RTD rtd_rear { 0.0f };

        sens::sht31::TempHum temp_hum_inside { 0, 0 };
        sens::sht31::TempHum temp_hum_outside { 0, 0 };

        const sens::max31865::FaultThreshold fault_threshold {
            sens::max31865::ADC_Code { sens::max31865::RTD(100.0f).adc_code },
            sens::max31865::ADC_Code { sens::max31865::RTD(-20.0f).adc_code }
        };

        const sens::max31865::Configuration configuration {
            sens::max31865::Masks::Configuration::Vbias::Or::ON,
            sens::max31865::Masks::Configuration::ConversionMode::Or::AUTO,
            sens::max31865::Masks::Configuration::WireMode::Or::TWO_WIRE_OR_FOUR_WIRE
        };
    private:
        sens::max31865::Transceiver max31865_transceiver_front { &hspi3, SPI3_TEMP_NSS0_GPIO_Port, SPI3_TEMP_NSS0_Pin };
        sens::max31865::Extension max31865_extension_front { SPI3_TEMP_NDRDY0_GPIO_Port, SPI3_TEMP_NDRDY0_Pin, max31865_transceiver_front };

        sens::max31865::Transceiver max31865_transceiver_rear { &hspi3, SPI3_TEMP_NSS1_GPIO_Port, SPI3_TEMP_NSS1_Pin };
        sens::max31865::Extension max31865_extension_rear { SPI3_TEMP_NDRDY1_GPIO_Port, SPI3_TEMP_NDRDY1_Pin, max31865_transceiver_rear };

        sens::sht31::Transceiver sht31_transceiver_outside { &hi2c1, sens::sht31::SlaveAddress7bit::ADDR_PIN_HIGH };
        sens::sht31::Extension sht31_extension_outside { sht31_transceiver_outside };

        sens::sht31::Transceiver sht31_transceiver_inside { &hi2c1, sens::sht31::SlaveAddress7bit::ADDR_PIN_LOW };
        sens::sht31::Extension sht31_extension_inside { sht31_transceiver_inside };
    private:
        SenserKiller() = default;
    public:
        static SenserKiller& get_instance();
    private:
        void init_max31865();
        void init_sht31();
        void init();
        static void worker(void* arg);
    public:
        osStatus release_semaphore_front();
        osStatus release_semaphore_rear();
    };
}