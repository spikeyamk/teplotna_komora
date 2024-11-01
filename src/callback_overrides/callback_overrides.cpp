#include <iostream>

#include "tim.h"
#include "gpio.h"
#include "bksram/bksram.hpp"
#include "rtc/rtc.hpp"
#include "comm/usb_uart/usb_uart.hpp"

extern "C" int __io_putchar(int ch) {
    return comm::usb_uart::__io_putchar(ch);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if(GPIO_Pin == BUT_USER_Pin) {
        rtc::set_time(10, 22, 0);
        rtc::set_date(24, 11, 1, 5);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM6) {
        std::printf("HAL_TIM_PeriodElapsedCallback: htim->Instance == TIM6: TWDG did not stop!\n");
        bksram::write<bksram::ErrorCodes::TWDG>();
    } else if (htim->Instance == TIM7) {
        HAL_IncTick();
    }
}