#include "stm32f2xx_hal.h"
#include "actu/buzzer/buzzer.hpp"

namespace actu {
namespace buzzer {
    void start() {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
    }

    void stop() {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
    }
}
}