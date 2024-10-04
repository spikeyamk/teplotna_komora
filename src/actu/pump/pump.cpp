#include "stm32f2xx_hal.h"
#include "actu/pump/pump.hpp"


namespace actu {
namespace pump {
    void start() {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
    }

    void stop() {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
    }
}
}