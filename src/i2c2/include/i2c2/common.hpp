#pragma once

#include <cstdint>
#include "stm32h7xx_hal.h"

namespace i2c2 {
    extern const uint32_t TIMEOUT;
    extern const GPIO_TypeDef* PORT;
    enum class Pins : uint16_t {
        SDA = GPIO_PIN_0,
        SCL = GPIO_PIN_1,
    };
    void Error_Handler();
}