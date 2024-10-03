#include <string_view>
#include <trielo/trielo.hpp>
#include "submodule/public.hpp"
#include "actu/fan/fan.hpp"
#include "actu/buzzer/buzzer.hpp"
#include "actu/lin_source/lin_source.hpp"
#include "actu/pump/pump.hpp"
#include "panel/sevseg/white/white.hpp"
#include "sens/i2c/common/common.hpp"
#include "stm32f2xx_hal.h"
#include "main.h"
#include "app.hpp"

/// This function calculates the area of a rectangle.
int app_main(int width, int height, TIM_HandleTypeDef* htim2, I2C_HandleTypeDef* hi2c1, DAC_HandleTypeDef* hdac) {
    (void) width;
    (void) height;

    /* STM32H503x has 128K FLASH only these functions don't fit into it */
    //Trielo::trielo<submodule::foo>();
    Trielo::trielo<actu::fan::stop_all>(htim2);
    std::printf("\n\r");
    sens::i2c::common::scan(hi2c1);
    actu::lin_source::start_dac(hdac);
    actu::lin_source::set_output(hdac, 1024, 1024);

    size_t i = 0;
    bool buzzer_running { false };
    while(1) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
        if(buzzer_running == false) {
            actu::buzzer::start();
            actu::pump::start();
            panel::sevseg::white::display();
            buzzer_running = true;
        } else {
            actu::buzzer::stop();
            actu::pump::stop();
            buzzer_running = false;
        }
        std::printf("%u: Hello World!\n\r", i++);
        HAL_Delay(500);
    }
    return 0;
}