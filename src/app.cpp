#include <trielo/trielo.hpp>
#include "submodule/public.hpp"
#include "returns_true.hpp"
#include "stm32f2xx_hal.h"
#include "app.hpp"

void fan_stop() {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);

    // Missing fan
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);

    // Missing fan
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
}

/// This function calculates the area of a rectangle.
int run(int width, int height) {
    (void) width;
    (void) height;
    /* STM32H503x has 128K FLASH only these functions don't fit into it */
    //Trielo::trielo<submodule::foo>();
    //Trielo::trielo<returns_true>();
    std::printf("Hello World!\n\r");
    return 0;
}