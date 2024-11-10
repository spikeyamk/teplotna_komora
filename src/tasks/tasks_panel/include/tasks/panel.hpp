#pragma once

#include <iostream>

#include "spi.h"
#include "sens/max31865/rtd.hpp"
#include "panel/sevseg/green_yellow/green_yellow.hpp"
#include "tasks/prototype.hpp"

namespace tasks {
    class Panel : public Prototype<Panel, 2 * 1024, "panel"> {
        friend CRTP;
    private:
        panel::sevseg::green_yellow::MAX6549 max6549 { &hspi2, SPI2_SEVYG_NSS_GPIO_Port, SPI2_SEVYG_NSS_Pin };
    public:
        const sens::max31865::RTD DESIRED_RTD_ENCODER_STEP { sens::max31865::ADC_Code(sens::max31865::RTD(1.0f).adc_code.value - sens::max31865::RTD(0.0f).adc_code.value).serialize() };
        static constexpr sens::max31865::RTD DESIRED_RTD_MAX { 85.0f };
        static constexpr sens::max31865::RTD DESIRED_RTD_MIN { -40.0f };
        sens::max31865::RTD desired_rtd { 20.0f };
    public:
        Panel() = default;
    public:
        static Panel& get_instance();
    private:
        static void worker(void* arg);
    public:
        void increment();
        void decrement();
    };
}