#include <iostream>
#include "stm32f2xx_hal.h"
#include "panel/button/button.hpp"
//#include "../../actu/fan/include/actu/fan/fan.hpp"

namespace panel {
namespace button {
    void left_most_button_press() {
        if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3) == GPIO_PIN_SET) {
            std::printf("Leftmost button pressed.\n");
        }
    }

    void left_middle_button_press() {
        if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2) == GPIO_PIN_SET) {
            std::printf("Left middle button pressed.\n");
        }
    }

    void right_middle_button_press() {
        if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1) == GPIO_PIN_SET) {
            std::printf("Right middle button pressed.\n");
        }
    }

    bool right_most_button_press() {
        if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1) == GPIO_PIN_SET) {
            //std::printf("Right middle button pressed.\n");
            return true;
            HAL_Delay(1000);
        }
        }
    }
}