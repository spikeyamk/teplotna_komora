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
#include "util/reset.hpp"
#include "util/redirect_stdout.hpp"
#include "bksram/magic.hpp"

void turn_every_annoying_peripheral_off() {
    actu::fan::init_ctl();
    actu::fan::stop_all();

    actu::pump::stop();
    actu::buzzer::stop();

    actu::bridge::a::turn_off();
    actu::bridge::b::turn_off();
}

/**
 * @brief App entry point. This function cannot exit.
 * @param arg is necessary in oder for app_main's function pointer to be of type osThreadFunc_t. Remains unused, nullptr is injected into it. DO NOT DEREFERENCE!
 */
extern "C" void app_main(void* arg) {
    (void) arg;
    if(Trielo::trielo<bksram::read>() != bksram::magic) {
        Trielo::trielo<util::reset>(bksram::magic);
    } else {
        Trielo::trielo<bksram::write>(0x00);
    }

    Trielo::trielo<turn_every_annoying_peripheral_off>();
    TRIELO_VOID(util::RedirectStdout::get_instance().init());

    Trielo::trielo<actu::fan::start_min_speed>();
    for(uint32_t tick = 0; true; tick++) {
        std::printf("app_main: tick: %lu\n", tick);
        osDelay(5000);
    }

    // we should never get here...
}