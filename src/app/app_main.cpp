#include <array>
#include <bitset>
#include <string_view>
#include <limits>
#include <trielo/trielo.hpp>

#include "cmsis_os2.h"

#include "iwdg.h"
#include "bksram/bksram.hpp"
#include "actu/fan/ctl/ctl.hpp"
#include "actu/fan/fb/fb.hpp"
#include "actu/bridge/bridge.hpp"
#include "actu/buzzer/buzzer.hpp"
#include "actu/lin_source/lin_source.hpp"
#include "actu/pump/pump.hpp"
#include "panel/sevseg/white/white.hpp"
#include "panel/encoder/encoder.hpp"
#include "panel/led/led.hpp"
#include "util/util.hpp"
#include "producer_consumer_test.hpp"
#include "comm/usb_uart/usb_uart.hpp"
#include "util/util.hpp"
#include "panel/sevseg/green_yellow/green_yellow.hpp"
#include "panel/led/led.hpp"
#include "sens/max31865/test.hpp"
#include "sens/sht31/test.hpp"

#include "tasks/fan_senser.hpp"
#include "tasks/panel.hpp"
#include "tasks/rs232_uart.hpp"
#include "tasks/temp_senser.hpp"
#include "tasks/temp_ctl.hpp"

void shutdown_endless_loop() {
    actu::fan::ctl::all::init();
    actu::fan::ctl::all::start_full_speed();

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

    const auto bksram_error_sevmap { panel::sevseg::common::uint20_t_to_sevmap(bksram::read()) };

    __disable_irq();
    while(1) {
        panel::sevseg::white::display_refresh_nodelay(bksram_error_sevmap);
        HAL_IWDG_Refresh(&hiwdg);
    }
}

/**
 * @brief App entry point. This function cannot exit.
 * @param arg is necessary in oder for app_main's function pointer to be of type osThreadFunc_t. Remains unused, nullptr is injected into it. DO NOT DEREFERENCE!
 */
extern "C" void app_main(void* arg) {
    (void) arg;

    /*
    if(bksram::test() == false) {
        shutdown_endless_loop();
    }
    */

    if(comm::usb_uart::RedirectStdout::get_instance().init_threadsafe() == false) {
        comm::usb_uart::RedirectStdout::get_instance().turn_off_threadsafe();
        std::printf("app_main: comm::usb_uart::RedirectStdout::get_instance().init_threadsafe() == false\n");
    }

    util::turn_every_annoying_peripheral_off();

    tasks::TempSenser::get_instance().init();
    if(tasks::TempSenser::get_instance().launch() == false) {
        bksram::write_reset<bksram::ErrorCodes::TempSenser::LAUNCH>();
    }

    /*
    actu::fan::ctl::all::start_min_speed();
    osDelay(10'000);
    tasks::FanSenser::get_instance().init();
    if(tasks::FanSenser::get_instance().launch() == false) {
        bksram::write_reset<bksram::ErrorCodes::FanSenser::LAUNCH>();
    }
    */

    if(tasks::Panel::get_instance().launch() == false) {
        bksram::write_reset<bksram::ErrorCodes::Panel::LAUNCH>();
    }

    tasks::RS232_UART::get_instance().init();
    if(tasks::RS232_UART::get_instance().launch() == false) {
        bksram::write_reset<bksram::ErrorCodes::RS232_UART::LAUNCH>();
    }

    /*
    if(tasks::TempCtl::get_instance().launch() == false) {
        bksram::write_reset<bksram::ErrorCodes::TempCtl::LAUNCH>();
    }
    */

    for(size_t tick = 10; true; tick++) {
        std::printf("app_main: tick: %zu\n", tick);
        osDelay(5'000);
    }

    // we should never get here...
}