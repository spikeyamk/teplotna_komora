#include <limits>
#include "actu/fan/fan.hpp"

namespace actu {
namespace fan {
    void stop_all(TIM_HandleTypeDef* htim2) {
        //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
        HAL_TIM_PWM_Start(htim2, TIM_CHANNEL_1);
        __HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_1, 50000);

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);

        // Missing fan
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);

        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);

        // Missing fan
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
    }
}
}