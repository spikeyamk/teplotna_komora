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
    void display_number(uint32_t number);
}
}
}