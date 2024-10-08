#pragma once

#include "stm32f2xx_hal.h"

namespace peripheral {
namespace gpio {
namespace i2cbb {
    extern const GPIO_TypeDef* PORT;

    enum class Pins {
        I2CBB1_SDA = GPIO_PIN_6,
        I2CBB1_SCL = GPIO_PIN_7,
    };

    void init();
    void deinit();
}
}
}