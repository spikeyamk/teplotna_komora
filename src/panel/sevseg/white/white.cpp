#include "stm32f2xx_hal.h"
#include "panel/sevseg/white/white.hpp"

namespace panel {
namespace sevseg {
namespace white {
    void init_brightness(TIM_HandleTypeDef* htim2) {
        HAL_TIM_PWM_Start(htim2, TIM_CHANNEL_1);
    }

    void dim(TIM_HandleTypeDef* htim2) {
        __HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_1, 50'000);
    }

    void bright(TIM_HandleTypeDef* htim2) {
        __HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_1, 60'000);
    }

    void turn_on_all_segments() {
        //brightness
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
        
        //cathodes
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);

        //segments
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
    }
}
}
}