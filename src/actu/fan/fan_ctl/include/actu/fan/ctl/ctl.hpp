#pragma once

#include "stm32f2xx_hal.h"

namespace actu {
namespace fan {
namespace ctl {
    void init();

    /**
     * Sets speed of all fans
     * @param speed_0_to_99 when 0 fan_fb 307 Hz, when 95 28 Hz, when greater than 95 fan stops
     */
    void set_speed(const uint8_t speed_0_to_99);

    /**
     * Stops all 6 available fans. Additionally sends stop signal to all 8 fan control
     * pins (2 fans not available at the moment).
    */
    void stop_all();

    void start_all();
    void start_all_half_speed();
    void start_all_min_speed();
    void test_speed();
}
}
}