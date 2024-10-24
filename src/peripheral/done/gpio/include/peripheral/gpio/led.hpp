#pragma once

#include "stm32f2xx_hal.h"

namespace peripheral {
namespace gpio {
namespace led {
    extern const GPIO_TypeDef* PORT;
    enum class Pins {
        COOL_LED = GPIO_PIN_0,
        HEAT_LED = GPIO_PIN_1,
        RS232_LED = GPIO_PIN_2,
        USB_LED = GPIO_PIN_3,
    };

    void init();
    void deinit();
}    
}
}