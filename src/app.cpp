#include <optional>
#include <array>
#include <trielo/trielo.hpp>
#include "stm32h7xx_hal.h"
#include "comm/usb_uart/usb_uart.hpp"
#include "cmsis_os2.h"
#include "submodule/public.hpp"
#include "i2c2/test.hpp"
#include "ds3231/test.hpp"
#include "app.hpp"

/// This function calculates the area of a rectangle.
void app_main(void* arg) {
    UNUSED(arg);
    comm::usb_uart::init();
    //i2c2::hw::test();
    //i2c2::stm32_bitbang_i2c::test();
    //i2c2::sw::test();
    for(size_t i = 0; true; i++) {
        //std::printf("%u: app_main\n", i);
        ds3231::test();
        osDelay(2'000);
    }
}