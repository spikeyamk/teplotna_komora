#pragma once

#include <array>
#include <iostream>

#include "main.h"
#include "spi.h"
#include "sens/max31865/max31865.hpp"
#include "tasks/prototype.hpp"

namespace tasks {
    class SenserKiller : public Prototype<SenserKiller, 8 * 1024, "senser_killer"> {
    public:
        float temp_front { 0.0f };
        float temp_rear { 0.0f };

        const sens::max31865::FaultThreshold fault_threshold {
            sens::max31865::ADC_Code { sens::max31865::RTD(100.0f).adc_code },
            sens::max31865::ADC_Code { sens::max31865::RTD(-30.0f).adc_code }
        };

        const sens::max31865::Configuration configuration {
            sens::max31865::Masks::Configuration::Vbias::Or::ON,
            sens::max31865::Masks::Configuration::ConversionMode::Or::AUTO,
            sens::max31865::Masks::Configuration::WireMode::Or::TWO_WIRE_OR_FOUR_WIRE
        };

        sens::max31865::Transceiver transceiver_front { &hspi3, SPI3_TEMP_NSS0_GPIO_Port, SPI3_TEMP_NSS0_Pin };
        sens::max31865::Extension extension_front { SPI3_TEMP_NDRDY0_GPIO_Port, SPI3_TEMP_NDRDY0_Pin, transceiver_front };

        sens::max31865::Transceiver transceiver_rear { &hspi3, SPI3_TEMP_NSS1_GPIO_Port, SPI3_TEMP_NSS1_Pin };
        sens::max31865::Extension extension_rear { SPI3_TEMP_NDRDY1_GPIO_Port, SPI3_TEMP_NDRDY1_Pin, transceiver_rear };

        panel::sevseg::green_yellow::MAX6549 max6549 { &hspi2, SPI2_SEVYG_NSS_GPIO_Port, SPI2_SEVYG_NSS_Pin };
    private:
        SenserKiller() = default;
    public:
        void init();
        static SenserKiller& get_instance();
        static void worker(void* arg);
    };
}