#pragma once

#include "stm32f2xx_hal.h"

namespace panel {
namespace sevseg {
namespace white {
    void init_brightness(TIM_HandleTypeDef* htim2);
    void dim(TIM_HandleTypeDef* htim2);
    void bright(TIM_HandleTypeDef* htim2);
    void turn_on_all_segments();
    void turn_off_all_segments();
    void display_digit(uint32_t digit);
    void display_one_number(const int digit, const uint16_t cathode_pin);
    void display_number(uint32_t number);
}
}
}