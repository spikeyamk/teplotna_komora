#include <optional>
#include <array>
#include <trielo/trielo.hpp>
#include "stm32h7xx_hal.h"
#include "cmsis_os2.h"
#include "submodule/public.hpp"
#include "i2c2/test.hpp"
#include "ds3231/test.hpp"
#include "app.hpp"

/// This function calculates the area of a rectangle.
void app_main(void* arg) {
    UNUSED(arg);
    /* STM32H503x has 128K FLASH only these functions don't fit into it */
    //Trielo::trielo<submodule::foo>();
    //Trielo::trielo<returns_true>();
    i2c2::hw::test();
    //i2c2::sw::test();
    //i2c2::stm32_bitbang_i2c::test();
    //ds3231::test();
    while(1) {
        std::printf("app_main\n");
        osDelay(1000);
    }
}
