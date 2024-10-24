#pragma once

#include "stm32f2xx_hal.h"
#include "panel/sevseg/common/common.hpp"

namespace panel {
namespace sevseg {
namespace white {    
    void init_brightness();
    void dim();
    void bright();
    void turn_on_all_segments();
    void display_pins();
    void launch_display_task(float& number);
    uint8_t set_digit(float number, uint8_t position);
    void display_refresh(const common::sevmap& sevmap);
}
}
}