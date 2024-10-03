#include "stm32f2xx_hal.h"
#include "actu/pump/pump.hpp"


namespace actu {
namespace pump {
    void pump_start() {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET);
    }

    void pump_stop() {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET);
    }
}
}