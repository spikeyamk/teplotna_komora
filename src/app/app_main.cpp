#include <array>
#include <bitset>
#include <string_view>
#include <limits>
#include <trielo/trielo.hpp>

#include "cmsis_os2.h"

#include "actu/fan/fan.hpp"
#include "actu/bridge/bridge.hpp"
#include "actu/buzzer/buzzer.hpp"
#include "actu/lin_source/lin_source.hpp"
#include "actu/pump/pump.hpp"
#include "panel/led/led.hpp"
#include "util/util.hpp"
#include "bksram/magic.hpp"
#include "comm/usb_uart/usb_uart.hpp"

/**
 * @brief App entry point. This function cannot exit.
 * @param arg is necessary in oder for app_main's function pointer to be of type osThreadFunc_t. Remains unused, nullptr is injected into it. DO NOT DEREFERENCE!
 */
extern "C" void app_main(void* arg) {
    (void) arg;
    comm::usb_uart::RedirectStdout& redirect_stdout { comm::usb_uart::RedirectStdout::get_instance() };
    if(redirect_stdout.init() == false) {
        redirect_stdout.turn_off_threadsafe();
        std::printf("app_main: redirect_stdout.init() == false\n");
    }

    /*
    if(Trielo::trielo<bksram::read>() != bksram::magic) {
        Trielo::trielo<util::reset>(bksram::magic);
    } else {
        Trielo::trielo<bksram::write>(0x00);
    }
    */

    Trielo::trielo<util::turn_every_annoying_peripheral_off>();
    for(uint32_t tick = 0; true; tick++) {
        std::printf("app_main: tick: %lu\n", tick);
        panel::led::toggle_all();
        osDelay(5000);
    }

    // we should never get here...
}