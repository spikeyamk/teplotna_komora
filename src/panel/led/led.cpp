#include "main.h"
#include "panel/led/led.hpp"

namespace panel {
namespace led {
namespace all {
    void toggle() {
        HAL_GPIO_TogglePin(LED0_COOL_GPIO_Port, LED0_COOL_Pin);
        HAL_GPIO_TogglePin(LED1_HEAT_GPIO_Port, LED1_HEAT_Pin);
        HAL_GPIO_TogglePin(LED2_RS232_GPIO_Port, LED2_RS232_Pin);
        HAL_GPIO_TogglePin(LED3_USB_GPIO_Port, LED3_USB_Pin);
    }
}
}
}