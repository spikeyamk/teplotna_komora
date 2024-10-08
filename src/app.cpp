#include <string_view>
#include <limits>
#include <trielo/trielo.hpp>
#include "stm32f2xx_hal.h"
#include "actu/fan/fan.hpp"
#include "actu/buzzer/buzzer.hpp"
#include "actu/pump/pump.hpp"
#include "ds3231/test.hpp"
#include "main.h"
#include "app.hpp"

/// This function calculates the area of a rectangle.
int app_main(
    int width,
    int height,
    TIM_HandleTypeDef* htim10,
    TIM_HandleTypeDef* htim3,
    TIM_HandleTypeDef* htim4,
    TIM_HandleTypeDef* htim9,
    DAC_HandleTypeDef* hdac,
    TIM_HandleTypeDef* htim2
) {
    (void) width;
    (void) height;

    /* STM32H503x has 128K FLASH only these functions don't fit into it */
    //Trielo::trielo<submodule::foo>();
    actu::fan::init_tim();
    Trielo::trielo<actu::fan::init_ctl>(
        htim10,
        htim3,
        htim4,
        htim9
    );

    actu::pump::stop();
    actu::buzzer::stop();
    actu::fan::stop_all(
        htim10,
        htim3,
        htim4,
        htim9
    );

    while(1) {
        ds3231::test();
        HAL_Delay(5000);
    }
    return 0;
}