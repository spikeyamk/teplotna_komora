#include <array>
#include <bitset>
#include <string_view>
#include <limits>
#include <trielo/trielo.hpp>

#include "example_subdirectory/public.hpp"
#include "actu/fan/fan.hpp"
#include "actu/bridge/bridge.hpp"
#include "actu/buzzer/buzzer.hpp"
#include "actu/lin_source/lin_source.hpp"
#include "actu/pump/pump.hpp"
#include "panel/sevseg/white/white.hpp"
#include "panel/encoder/encoder.hpp"
#include "app/app_main.hpp"

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
    Trielo::trielo<example_subdirectory::foo>();
    turn_every_annoying_peripheral_off();

    actu::fan::start_min_speed();

    actu::lin_source::test_dac();
    
    for (;;) {
        panel::sevseg::white::display_refresh(12345);
        panel::encoder::encoder_test();
    }

    /*for(uint32_t tick = 0; true; tick++) {
        std::printf("app_main: tick: %lu\n", tick);
        HAL_Delay(5000);
    }*/
}