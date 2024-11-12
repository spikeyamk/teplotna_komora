#pragma once

#include <ranges>

#include "stm32f2xx_hal.h"
#include "util/util.hpp"
#include "panel/sevseg/common/common.hpp"

namespace panel {
namespace sevseg {
namespace white {    
    extern const std::array<uint16_t, 8> segment_pins;
    extern const std::array<uint16_t, 5> common_cathode_pins;
    
    void init();
    void dim();
    void bright();
    void turn_on_all_segments();
    void display_pins();
    uint8_t set_digit(float number, uint8_t position);

    template<auto waiting_functor>
    inline void refresh(const common::sevmap& sevmap) {
        for(const auto& [sevset, cathode_pin] : std::ranges::views::zip(sevmap, common_cathode_pins)) {
            for(size_t i = 0; i < sevset.size(); i++) {
                HAL_GPIO_WritePin(GPIOE, segment_pins[i], sevset[i] == true ? GPIO_PIN_SET : GPIO_PIN_RESET);
            }
            HAL_GPIO_WritePin(GPIOE, cathode_pin, GPIO_PIN_RESET);
            waiting_functor();
            HAL_GPIO_WritePin(GPIOE, cathode_pin, GPIO_PIN_SET);
        }
    }
}
}
}