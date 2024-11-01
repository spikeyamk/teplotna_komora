#include <iostream>

#include "tim.h"
#include "comm/usb_uart/usb_uart.hpp"

extern "C" int __io_putchar(int ch) {
    return comm::usb_uart::__io_putchar(ch);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM6) {
        std::printf("HAL_TIM_PeriodElapsedCallback: htim->Instance == TIM6: TWDG did not stop!\n");
    } else if (htim->Instance == TIM7) {
        HAL_IncTick();
    }
}