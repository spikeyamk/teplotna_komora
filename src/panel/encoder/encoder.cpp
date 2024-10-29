#include <iostream>
#include "main.h"
#include "panel/encoder/encoder.hpp"
#include "stm32f2xx_hal.h"

extern float counter = 0.0f; //move to display (common)
uint8_t rot_get_state() {
    return (uint8_t)
}
extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    switch(GPIO_Pin) {
        case ENCA_EXTI10_Pin:
            if (GPIO_PIN_)
            break;
        case ENCB_EXTI11_Pin: //need to add interrupt
            break;
        case BUT0_FR_Pin:
            break;
        case BUT1_MR_Pin:
            break;
        case BUT2_ML_Pin:
            break;
        case BUT3_FL_Pin:
            break;
    }
};

namespace panel {
namespace encoder {
    
    /*void encoder_test() {
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
    }*/
}
}
