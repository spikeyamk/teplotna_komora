#pragma once

#include <iostream>
#include "main.h"
#include "stm32f2xx_hal.h"

namespace panel {
namespace encoder {
    inline void test() {
        std::printf(
            "HAL_GPIO_ReadPin(ENCA_EXTI10_GPIO_Port, ENCA_EXTI10_Pin): 0x%02X\n",
            HAL_GPIO_ReadPin(ENCA_EXTI10_GPIO_Port, ENCA_EXTI10_Pin)
        );
        std::printf(
            "HAL_GPIO_ReadPin(ENCB_EXTI11_GPIO_Port, ENCB_EXTI11_Pin): 0x%02X\n",
            HAL_GPIO_ReadPin(ENCB_EXTI11_GPIO_Port, ENCB_EXTI11_Pin)
        );
    }
}
}
