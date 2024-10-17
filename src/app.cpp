#include <trielo/trielo.hpp>
#include "submodule/public.hpp"
#include "fanctl/bulk.hpp"
#include "returns_true.hpp"
#include "stm32f2xx_hal.h"
#include "app.hpp"

/// This function calculates the area of a rectangle.
int run(int width, int height) {
    (void) width;
    (void) height;

    /* STM32H503x has 128K FLASH only these functions don't fit into it */
    Trielo::trielo<submodule::foo>();
    Trielo::trielo<returns_true>();

    fanctl::stop_all();

    while(1) {
        /* USER CODE END WHILE */
        /* USER CODE BEGIN 3 */
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
        HAL_Delay(500);
    }
    return 0;
}