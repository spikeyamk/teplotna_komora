#pragma once

#include "stm32f2xx_hal.h"
#include <bitset>

namespace panel {
namespace sevseg {
namespace white {
    extern const std::array<uint8_t, 8> active_segment;
    extern const std::array<uint8_t, 5> active_cathodes;
    
    void init_brightness();
    void dim();
    void bright();
    void brightness_on();
    void brightness_off();
    void turn_on_all_segments();
    void display_pins();
    uint8_t set_digit(float number, uint8_t position);
    void display_refresh();
}
}
}