#pragma once

#include "stm32f2xx_hal.h"
#include "panel/sevseg/common/common.hpp"

namespace panel {
namespace sevseg {
namespace white {    
    extern const std::array<uint16_t, 8> active_segment;
    extern const std::array<uint16_t, 5> active_cathodes;
    
    void init_brightness();
    void dim();
    void bright();
    void turn_on_all_segments();
    void display_pins();
    void launch_display_task(float& number);
    uint8_t set_digit(float number, uint8_t position);

    inline void display_refresh(const common::sevmap& sevmap) {
        for(size_t i = 0; i < sevmap.size(); i++) {
            for(size_t j = 0; j < sevmap[i].size(); j++) {
                HAL_GPIO_WritePin(GPIOE, active_segment[j], sevmap[i][j] == true ? GPIO_PIN_SET : GPIO_PIN_RESET);
            }
            HAL_GPIO_WritePin(GPIOE, active_cathodes[i], GPIO_PIN_RESET);
            HAL_Delay(1);
            HAL_GPIO_WritePin(GPIOE, active_cathodes[i], GPIO_PIN_SET);
        }
    }
}
}
}