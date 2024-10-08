#pragma once

#include "peripheral/gpio/pump.hpp"

namespace peripheral {
namespace gpio {
namespace pump {
    const GPIO_TypeDef* PORT { GPIOD };
    const uint16_t PIN { GPIO_PIN_11 };

    void init() {
        __HAL_RCC_GPIOD_CLK_ENABLE();
        HAL_GPIO_WritePin(const_cast<GPIO_TypeDef*>(PORT), PIN, GPIO_PIN_RESET);
        const GPIO_InitTypeDef GPIO_InitStruct {
            .Pin = PIN,
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW,
        };
        HAL_GPIO_Init(const_cast<GPIO_TypeDef*>(PORT), const_cast<GPIO_InitTypeDef*>(&GPIO_InitStruct));
    }

    void deinit() {
        HAL_GPIO_DeInit(const_cast<GPIO_TypeDef*>(PORT), PIN);
    }
}    
}
}