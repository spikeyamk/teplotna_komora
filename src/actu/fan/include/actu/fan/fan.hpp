#pragma once

#include "stm32f2xx_hal.h"

namespace actu {
namespace fan {
    void init(
        TIM_HandleTypeDef* htim5,
        TIM_HandleTypeDef* htim10,
        TIM_HandleTypeDef* htim3,
        TIM_HandleTypeDef* htim4,
        TIM_HandleTypeDef* htim9
    );

    /**
     * Stops all 6 available fans. Additionally sends stop signal to all 8 fan control
     * pins (2 fans not available at the moment).
    */
    void stop_all(
        TIM_HandleTypeDef* htim5,
        TIM_HandleTypeDef* htim10,
        TIM_HandleTypeDef* htim3,
        TIM_HandleTypeDef* htim4,
        TIM_HandleTypeDef* htim9
    );

    void start_all(
        TIM_HandleTypeDef* htim5,
        TIM_HandleTypeDef* htim10,
        TIM_HandleTypeDef* htim3,
        TIM_HandleTypeDef* htim4,
        TIM_HandleTypeDef* htim9
    );
}
}