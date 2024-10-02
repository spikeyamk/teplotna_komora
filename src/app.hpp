#pragma once

#include "stm32f2xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function calculates the area of a rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @return The area of the rectangle.
 */
int app_main(int width, int height, UART_HandleTypeDef* huart1);

#ifdef __cplusplus
}
#endif