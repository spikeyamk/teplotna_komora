#include "stm32f2xx_hal.h"
#include <iostream>
#include "main.h"
#include "panel/encoder/encoder.hpp"

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    switch(GPIO_Pin) {
        case ENCA_EXTI10_Pin:
            panel::encoder::enca_ext10_handler();
            break;
        case ENCB_EXTI11_Pin:
            panel::encoder::encb_ext11_handler();
            break;
        case BUT0_FR_Pin:
            break;
        case BUT1_MR_Pin:
            break;
        case BUT2_ML_Pin:
            break;
        case BUT3_FL_Pin:
            break;
        case SPI3_TEMP_NDRDY0_Pin:
            //std::printf("NDRDY0\n");
            break;
        case SPI3_TEMP_NDRDY1_Pin:
            //std::printf("NDRDY1\n");
            break;
    }
}