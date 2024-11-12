#include "actu/fan/ctl/ctl.hpp"
#include "actu/pump/pump.hpp"
#include "actu/buzzer/buzzer.hpp"
#include "actu/peltier/peltier.hpp"
#include "panel/sevseg/white/white.hpp"
#include "iwdg.h"
#include "tim.h"
#include "bksram/bksram.hpp"
#include "util/util.hpp"

namespace util {
    void turn_every_annoying_peripheral_off() {
        actu::fan::ctl::all::stop();
        actu::pump::stop();
        actu::buzzer::stop();
        actu::peltier::front::set(0);
        actu::peltier::rear::set(0);
    }

    void shutdown_endless_loop() {
        actu::fan::ctl::all::init();
        actu::fan::ctl::all::start_min_speed();

        actu::pump::stop();
        actu::buzzer::stop();

        actu::peltier::front::set(0);
        actu::peltier::rear::set(0);

        panel::sevseg::white::init();
        panel::sevseg::white::bright();

        const auto bksram_error_sevmap { panel::sevseg::common::to_sevmap(bksram::read()) };

        //__disable_irq();
        while(1) {
            panel::sevseg::white::refresh<[]() { for(uint32_t i = 0; i < 1'000; i++) { asm("nop"); } }>(bksram_error_sevmap);
            HAL_IWDG_Refresh(&hiwdg);
        }
    }

    HAL_StatusTypeDef twdg_init() {
        __HAL_TIM_SET_COUNTER(&htim6, 0);
        __HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);
        return HAL_TIM_Base_Start_IT(&htim6);
    }
}