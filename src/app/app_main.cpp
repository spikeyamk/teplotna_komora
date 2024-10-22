#include <array>
#include <bitset>
#include <string_view>
#include <limits>
#include <trielo/trielo.hpp>

#include "actu/fan/fan.hpp"
#include "actu/bridge/bridge.hpp"
#include "actu/buzzer/buzzer.hpp"
#include "actu/lin_source/lin_source.hpp"
#include "actu/pump/pump.hpp"
#include "app/app_main.hpp"
#include "rtc/rtc.hpp"

void turn_every_annoying_peripheral_off() {
    actu::fan::init_ctl();
    actu::fan::stop_all();

    actu::pump::stop();
    actu::buzzer::stop();

    actu::bridge::a::turn_off();
    actu::bridge::b::turn_off();
}

/// This function calculates the area of a rectangle.
void app_main(void* arg) {
    (void) arg;
    turn_every_annoying_peripheral_off();
    //Trielo::trielo<rtc::set_time>(11, 27, 0);
    //Trielo::trielo<rtc::set_date>(24, 10, 22, 2);

    actu::fan::start_min_speed();
    rtc::get_time_date_test();

    for(uint32_t tick = 0; true; tick++) {
        std::printf("app_main: tick: %lu\n", tick);
        HAL_Delay(5000);
    }
}