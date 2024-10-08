#pragma once

#include "stm32f2xx_hal.h"

namespace peripheral {
namespace gpio {
namespace sevw {
    extern const GPIO_TypeDef* PORT;

    enum class Pins {
        SEVW_CC0_Pin = GPIO_PIN_0,
        SEVW_CC1_Pin = GPIO_PIN_1,
        SEVW_CC2_Pin = GPIO_PIN_2,
        SEVW_CC3_Pin = GPIO_PIN_3,
        SEVW_CC4_Pin = GPIO_PIN_4,

        SEVW_A_Pin = GPIO_PIN_7,
        SEVW_B_Pin = GPIO_PIN_8,
        SEVW_D_Pin = GPIO_PIN_10,
        SEVW_E_Pin = GPIO_PIN_11,
        SEVW_F_Pin = GPIO_PIN_12,
        SEVW_G_Pin = GPIO_PIN_13,
        SEVW_DP_Pin = GPIO_PIN_14,
    };

    void init();
    void deinit();
}    
}
}