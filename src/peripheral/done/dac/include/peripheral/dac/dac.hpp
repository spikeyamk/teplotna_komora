#pragma once

#include "stm32f2xx_hal.h"

void HAL_DAC_MspInit(DAC_HandleTypeDef* dacHandle);
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* dacHandle);

namespace peripheral {
namespace dac {
    extern const GPIO_TypeDef* PORT;
    extern DAC_HandleTypeDef hdac;
    enum class Pins {
        LINSA = GPIO_PIN_4,
        LINSB = GPIO_PIN_5,
    };
    void init();
    void deinit();
}
}