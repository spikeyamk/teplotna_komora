#include "main.h"
#include "stm32f2xx_hal.h"
#include "actu/pump/pump.hpp"

namespace actu {
namespace pump {
    void start() {
        HAL_GPIO_WritePin(NPUMPEN_GPIO_Port, NPUMPEN_Pin, GPIO_PIN_RESET);
    }

    void stop() {
        HAL_GPIO_WritePin(NPUMPEN_GPIO_Port, NPUMPEN_Pin, GPIO_PIN_SET);
    }
}
}