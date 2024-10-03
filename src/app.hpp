#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This function calculates the area of a rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @return The area of the rectangle.
 */
int app_main(int width, int height, TIM_HandleTypeDef* htim2, I2C_HandleTypeDef* hi2c1);

#ifdef __cplusplus
}
#endif