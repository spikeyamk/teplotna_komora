#include <limits>
#include "actu/fan/fan.hpp"

namespace actu {
namespace fan {
    void init(
        TIM_HandleTypeDef* htim5,
        TIM_HandleTypeDef* htim10,
        TIM_HandleTypeDef* htim3,
        TIM_HandleTypeDef* htim4,
        TIM_HandleTypeDef* htim9
    ) {
        HAL_TIM_PWM_Start(htim5, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(htim5, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start(htim10, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(htim3, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(htim3, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start(htim4, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(htim4, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start(htim9, TIM_CHANNEL_1);
    }

    void stop_all(
        TIM_HandleTypeDef* htim5,
        TIM_HandleTypeDef* htim10,
        TIM_HandleTypeDef* htim3,
        TIM_HandleTypeDef* htim4,
        TIM_HandleTypeDef* htim9
    ) {
        __HAL_TIM_SET_COMPARE(htim5, TIM_CHANNEL_1, 65535);
        __HAL_TIM_SET_COMPARE(htim5, TIM_CHANNEL_3, 65535);
        __HAL_TIM_SET_COMPARE(htim10, TIM_CHANNEL_1, 65535);
        __HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_1, 65535);
        __HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_3, 65535);
        __HAL_TIM_SET_COMPARE(htim4, TIM_CHANNEL_1, 65535);
        __HAL_TIM_SET_COMPARE(htim4, TIM_CHANNEL_3, 65535);
        __HAL_TIM_SET_COMPARE(htim9, TIM_CHANNEL_1, 65535);
    }

    void start_all(
        TIM_HandleTypeDef* htim5,
        TIM_HandleTypeDef* htim10,
        TIM_HandleTypeDef* htim3,
        TIM_HandleTypeDef* htim4,
        TIM_HandleTypeDef* htim9
    ) {
        __HAL_TIM_SET_COMPARE(htim5, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(htim5, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(htim10, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(htim4, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(htim4, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(htim9, TIM_CHANNEL_1, 0);
    }
}
}