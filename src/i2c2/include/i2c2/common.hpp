#pragma once

#include <cstdint>
#include "stm32h7xx_hal.h"

namespace i2c2 {
    extern const uint32_t TIMEOUT;
    extern const GPIO_TypeDef* PORT;
    enum class Pins : uint16_t {
        SDA = 0,
        SCL = 1,
    };
    void Error_Handler();
}