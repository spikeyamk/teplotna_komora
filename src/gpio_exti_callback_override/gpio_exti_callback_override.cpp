#include "stm32f2xx_hal.h"
#include <iostream>
#include "main.h"
#include "panel/encoder/encoder.hpp"

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    GPIO_PinState encoder_A = HAL_GPIO_ReadPin(ENCA_EXTI10_GPIO_Port, ENCA_EXTI10_Pin);
    GPIO_PinState encoder_B = HAL_GPIO_ReadPin(ENCB_EXTI11_GPIO_Port, ENCB_EXTI11_Pin);
    switch(GPIO_Pin) {
        case ENCA_EXTI10_Pin:
            //panel::encoder::test();
            if ((encoder_A == GPIO_PIN_SET) && (encoder_B == GPIO_PIN_RESET)) {std::printf("CW\n");}
            else if ((encoder_A == GPIO_PIN_RESET) && (encoder_B == GPIO_PIN_SET)) {std::printf("CW\n");}
            else if ((encoder_A == GPIO_PIN_RESET) && (encoder_B == GPIO_PIN_RESET)) {std::printf("ACW\n");}
            else if ((encoder_A == GPIO_PIN_SET) && (encoder_B == GPIO_PIN_SET)) {std::printf("ACW\n");}
            break;
        case ENCB_EXTI11_Pin:
            //panel::encoder::test();
            if ((encoder_A == GPIO_PIN_SET) && (encoder_B == GPIO_PIN_RESET)) {std::printf("ACW\n");}
            else if ((encoder_A == GPIO_PIN_RESET) && (encoder_B == GPIO_PIN_SET)) {std::printf("ACW\n");}
            else if ((encoder_A == GPIO_PIN_RESET) && (encoder_B == GPIO_PIN_RESET)) {std::printf("CW\n");}
            else if ((encoder_A == GPIO_PIN_SET) && (encoder_B == GPIO_PIN_SET)) {std::printf("CW\n");}
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
}