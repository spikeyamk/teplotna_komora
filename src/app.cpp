#include <trielo/trielo.hpp>
#include "stm32h5xx_hal.h"
#include "submodule/public.hpp"
#include "returns_true.hpp"
#include "app.hpp"

/// This function calculates the area of a rectangle.
int run(int width, int height) {
    (void) width;
    (void) height;
    //Trielo::trielo<submodule::foo>();
    //Trielo::trielo<returns_true>();
    std::printf("Hello World!\n");

    //HAL_Init();
    while (1) {
	    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        for(int i = 0; i < 1'000'000; i++) {
            std::printf("Hello World!\n");
        }
	    //HAL_Delay(250);
    }
    return 0;
}