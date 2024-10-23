#include "main.h"
#include "stm32f2xx_hal.h"
#include "actu/buzzer/buzzer.hpp"

namespace actu {
namespace buzzer {
    void start() {
        HAL_GPIO_WritePin(BUZZEN_GPIO_Port, BUZZEN_Pin, GPIO_PIN_SET);
    }

    void stop() {
        HAL_GPIO_WritePin(BUZZEN_GPIO_Port, BUZZEN_Pin, GPIO_PIN_RESET);
    }
}
}