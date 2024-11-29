#include "main.h"
#include "actu/fan/broiler/broiler.hpp"

namespace actu {
namespace fan {
namespace broiler {
    void start() {
        HAL_GPIO_WritePin(BROILEN_GPIO_Port, BROILEN_Pin, GPIO_PIN_RESET);
    }

    void stop() {
        HAL_GPIO_WritePin(BROILEN_GPIO_Port, BROILEN_Pin, GPIO_PIN_SET);
    }
}
}
}