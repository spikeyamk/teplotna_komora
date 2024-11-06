#include "stm32f2xx_hal.h"
#include <iostream>
#include "main.h"
#include "panel/encoder/encoder.hpp"
#include "actu/fan/fb/fb.hpp"
#include "comm/usb_uart/usb_uart.hpp"
#include "bksram/bksram.hpp"
#include "tasks/temp_senser.hpp"

extern "C" int __io_putchar(int ch) {
    return comm::usb_uart::__io_putchar(ch);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
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
            tasks::TempSenser::get_instance().release_semaphore_front();
            break;
        case SPI3_TEMP_NDRDY1_Pin:
            tasks::TempSenser::get_instance().release_semaphore_rear();
            break;
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    actu::fan::fb::HAL_TIM_IC_CaptureCallback(htim);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
    actu::fan::fb::HAL_TIM_PWM_PulseFinishedCallback(htim);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM1) {
        HAL_IncTick();
    } else if(htim->Instance == TIM6) {
        std::printf("HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim): htim->Instance == TIM6: TWDG did not stop!\n");
        bksram::write<bksram::ErrorCodes::TWDG>();
    }
}