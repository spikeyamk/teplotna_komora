#pragma once

#include "stm32f2xx_hal.h"

namespace actu {
namespace fan {
    /**
     * Stops all 6 available fans. Additionally sends stop signal to all 8 fan control
     * pins (2 fans not available at the moment).
    */
    void stop_all(TIM_HandleTypeDef* htim2);
}
}