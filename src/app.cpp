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
#include "sens/spi_temp/spi_temp.hpp"
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
    TIM_HandleTypeDef* htim2,
    SPI_HandleTypeDef* hspi2
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
    std::printf("\n\r");
    panel::sevseg::white::init_brightness(htim2);
    panel::sevseg::white::turn_on_all_segments();
    actu::pump::stop();
    actu::buzzer::stop();
    panel::sevseg::green_yellow::init();
    panel::sevseg::green_yellow::test();
    sens::spi_temp::test();

    actu::bridge::a::turn_off();
    actu::bridge::b::turn_off();
    actu::lin_source::start_dac(hdac);
    actu::lin_source::set_output(hdac, std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max());
    actu::bridge::b::cool();

    size_t i = 0;
    bool buzzer_running { false };
    while(1) {
        //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
        //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
        //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);
        //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
        /*
        if(buzzer_running == false) {
            actu::fan::start_all(
                htim10,
                htim3,
                htim4,
                htim9
            );
            actu::buzzer::start();
            actu::pump::start();
            panel::sevseg::white::dim(htim2);
            HAL_Delay(2000);
            //std::printf("fan_rpm: %lu\n\r", fan_rpm);
            buzzer_running = true;
        } else {
            actu::fan::stop_all(
                htim10,
                htim3,
                htim4,
                htim9
            );
            actu::buzzer::stop();
            panel::sevseg::white::bright(htim2);
            HAL_Delay(2000);
            //std::printf("fan_rpm: %lu\n\r", fan_rpm);
            buzzer_running = false;
        }
        */
        std::printf("%u: Hello World!\n\r", i++);
        HAL_Delay(2000);
    }
    return 0;
}