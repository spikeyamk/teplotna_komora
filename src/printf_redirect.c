#include "stm32f2xx_hal.h"

extern UART_HandleTypeDef huart1;

int __io_putchar(int ch) {
    if (ch == '\n') {
        HAL_UART_Transmit(&huart1, (const uint8_t*) "\r\n", 2, 1000);  // Send both at once
    } else {
        HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);  // Send character
    }
    return ch;
}