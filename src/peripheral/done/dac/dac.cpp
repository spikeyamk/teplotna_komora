#include <iostream>
#include "peripheral/dac/dac.hpp"

namespace peripheral {
namespace dac {
    const GPIO_TypeDef* PORT { GPIOA };
    DAC_HandleTypeDef hdac {};

    void Error_Handler() {
        std::cout << "peripheral::dac::Error_Handler\n";
    }

    void init() {
        DAC_ChannelConfTypeDef sConfig = {0};
        hdac.Instance = DAC;
        if(HAL_DAC_Init(&hdac) != HAL_OK) {
            Error_Handler();
        }

        sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
        sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
        if(HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK) {
            Error_Handler();
        }

        if(HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK) {
            Error_Handler();
        }
    }

    void deinit() {
        HAL_DAC_DeInit(&hdac);
    }
}
}