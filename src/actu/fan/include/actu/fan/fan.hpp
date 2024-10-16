#pragma once

#include "stm32f2xx_hal.h"

extern uint32_t fan_rpm;

namespace actu {
namespace fan {
    void init_ctl();
    void init_fb();
    void init_tim();

    /**
     * Stops all 6 available fans. Additionally sends stop signal to all 8 fan control
     * pins (2 fans not available at the moment).
    */
    void stop_all();
    void start_all();
    void start_all_half_speed();
}
}