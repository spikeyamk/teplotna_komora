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
int run(int width, int height);

/**
 * Stops all fans
*/
void fan_stop();

#ifdef __cplusplus
}
#endif