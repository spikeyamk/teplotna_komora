#include "actu/fan/ctl/ctl.hpp"
#include "actu/pump/pump.hpp"
#include "actu/buzzer/buzzer.hpp"
#include "actu/bridge/bridge.hpp"
#include "actu/lin_source/lin_source.hpp"
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
        actu::bridge::front::turn_off();
        actu::bridge::rear::turn_off();
        actu::lin_source::front::set_output(0);
        actu::lin_source::front::stop_dac();
        actu::lin_source::rear::set_output(0);
        actu::lin_source::front::stop_dac();
    }

    void shutdown_endless_loop() {
        actu::fan::ctl::all::init();
        actu::fan::ctl::all::start_min_speed();

        actu::pump::stop();
        actu::buzzer::stop();
        
        actu::lin_source::front::set_output(0);
        actu::lin_source::front::stop_dac();

        actu::lin_source::rear::set_output(0);
        actu::lin_source::rear::stop_dac();

        actu::bridge::front::turn_off();
        actu::bridge::rear::turn_off();

        panel::sevseg::white::init_brightness();
        panel::sevseg::white::bright();

        const auto bksram_error_sevmap { panel::sevseg::common::to_sevmap(bksram::read()) };

        //__disable_irq();
        while(1) {
            panel::sevseg::white::display_refresh_nodelay(bksram_error_sevmap);
            HAL_IWDG_Refresh(&hiwdg);
        }
    }

    HAL_StatusTypeDef twdg_init() {
        __HAL_TIM_SET_COUNTER(&htim6, 0);
        __HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);
        return HAL_TIM_Base_Start_IT(&htim6);
    }
}