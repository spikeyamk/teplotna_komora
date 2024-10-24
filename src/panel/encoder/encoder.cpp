#include <iostream>
#include "panel/encoder/encoder.hpp"
#include "stm32f2xx_hal.h"

namespace panel {
namespace encoder {
    
    void encoder_test() {
        int counter = 0;
        
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET) {
            if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_RESET) {

                while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_RESET) {};
                std::printf("Counter: %d\n", counter++);
                while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET) {};
                HAL_Delay(10);
            }

            if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_SET) {

                while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_SET) {};
                std::printf("Counter: %d\n", counter--);
                while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET) {};
                while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_RESET) {};
                HAL_Delay(10);
            }

            if (counter < 0) counter = 0;
            if (counter > 16) counter = 16;
    }
    }
}
}
