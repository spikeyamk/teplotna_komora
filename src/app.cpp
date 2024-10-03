#include <string_view>
#include <trielo/trielo.hpp>
#include "submodule/public.hpp"
#include "actu/fan/fan.hpp"
#include "stm32f2xx_hal.h"
#include "main.h"
#include "app.hpp"

/// This function calculates the area of a rectangle.
int app_main(int width, int height) {
    (void) width;
    (void) height;

    /* STM32H503x has 128K FLASH only these functions don't fit into it */
    Trielo::trielo<submodule::foo>();
    Trielo::trielo<actu::fan::stop_all>();

    size_t i = 0;
    while(1) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
        std::printf("%u: Hello World!\n\r", i++);
        HAL_Delay(500);
    }
    return 0;
}