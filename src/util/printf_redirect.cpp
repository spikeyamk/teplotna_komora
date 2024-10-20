#include "stm32f2xx_hal.h"
#include "usart.h"

extern "C" int __io_putchar(int ch) {
    if (ch == '\n') {
        HAL_UART_Transmit(&huart1, reinterpret_cast<const uint8_t*>("\r\n"), 2, 1000);
        HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>("\r\n"), 2, 1000);
    } else {
        HAL_UART_Transmit(&huart1, reinterpret_cast<uint8_t *>(&ch), 1, 1000);
        HAL_UART_Transmit(&huart3, reinterpret_cast<uint8_t *>(&ch), 1, 1000);
    }
    return ch;
}