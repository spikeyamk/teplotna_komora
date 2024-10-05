#include "stm32f2xx_hal.h"

extern UART_HandleTypeDef huart1;

int __io_putchar(int ch) {
    static int before_ch;
    before_ch = ch;
    HAL_UART_Transmit(&huart1, (uint8_t *) &ch, sizeof(uint8_t), 1000);
    return ch;
}