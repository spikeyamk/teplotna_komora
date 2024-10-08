#include <string_view>
#include <limits>
#include <trielo/trielo.hpp>
#include "submodule/public.hpp"
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
    panel::sevseg::white::init_brightness(htim2);
    panel::sevseg::white::turn_on_all_segments();
    actu::pump::stop();
    actu::buzzer::stop();
    /*
    actu::fan::start_all(
        htim10,
        htim3,
        htim4,
        htim9
    );
    */
    actu::fan::stop_all(
        htim10,
        htim3,
        htim4,
        htim9
    );

    //panel::sevseg::green_yellow::init();
    panel::sevseg::green_yellow::test();

    actu::bridge::a::turn_off();
    actu::bridge::b::turn_off();
    actu::lin_source::start_dac(hdac);
    actu::lin_source::set_output(hdac, std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max());
    //actu::bridge::a::cool();
    //actu::bridge::b::cool();

    for(uint16_t dac_value = 0; true; ) {
        HAL_DAC_SetValue(hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value);
        HAL_DAC_SetValue(hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac_value);
        if(dac_value + 1000 > 4095)
            dac_value = 0;
        else {
            dac_value += 1000;
        }
        std::printf("dac_value: %u\n", dac_value);
        HAL_Delay(5000);
    }
    return 0;
}