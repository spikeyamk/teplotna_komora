#pragma once

#include "stm32f2xx_hal.h"

extern uint32_t fan_rpm;

namespace actu {
namespace fan {
    void init_ctl();
    void init_fb();
    void init_tim();

    /**
     * Sets speed of all fans
     * @param speed_0_to_100 when 0 fan_fb 307 Hz, when 95 28 Hz, when greater than 95 fan stops
     */
    void set_speed(const uint8_t speed_0_to_100);
    void test_speed();
    /**
     * Stops all 6 available fans. Additionally sends stop signal to all 8 fan control
     * pins (2 fans not available at the moment).
    */
    void stop_all();
    void start_all();
    void start_all_half_speed();
    void start_min_speed();
}
}