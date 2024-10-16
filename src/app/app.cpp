#include <string_view>
#include <limits>
#include <trielo/trielo.hpp>
#include "example_submodule/public.hpp"
#include "actu/fan/fan.hpp"
#include "actu/bridge/bridge.hpp"
#include "actu/buzzer/buzzer.hpp"
#include "actu/lin_source/lin_source.hpp"
#include "actu/pump/pump.hpp"
#include "panel/sevseg/white/white.hpp"
#include "panel/sevseg/green_yellow/green_yellow.hpp"
#include "sens/i2c/common/common.hpp"
//#include "sens/spi_temp/spi_temp.hpp"
#include "stm32f2xx_hal.h"
#include "main.h"
#include "app/app.hpp"
#include "dac.h"

/// This function calculates the area of a rectangle.
void app_main(void* arg) {
    (void) arg;

    /* STM32H503x has 128K FLASH only these functions don't fit into it */
    //Trielo::trielo<submodule::foo>();

    actu::fan::init_tim();
    Trielo::trielo<actu::fan::init_ctl>();
    panel::sevseg::white::init_brightness();
    panel::sevseg::white::turn_on_all_segments();
    actu::pump::stop();
    actu::buzzer::stop();
    actu::fan::stop_all();

    actu::bridge::a::turn_off();
    actu::bridge::b::turn_off();
    actu::lin_source::start_dac();
    actu::lin_source::set_output(std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max());

    //actu::bridge::a::cool();
    //actu::bridge::b::cool();

    for(
        uint16_t dac_value = 0;
        true;
        dac_value = [](const uint16_t in) {
            static constexpr uint16_t inc { 2 << 9 };
            static constexpr uint16_t stopper { 2 << 11 };
            return (in + inc) > stopper ? 0 : in + inc;
        }(dac_value)
    ) {
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value);
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac_value);
        std::printf("dac_value: %u\n", dac_value);
        HAL_Delay(5000);
    }
}