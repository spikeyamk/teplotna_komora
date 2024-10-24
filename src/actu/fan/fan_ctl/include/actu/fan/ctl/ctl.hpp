#pragma once

#include "actu/fan/common/common.hpp"

namespace actu {
namespace fan {
namespace ctl {
    HAL_StatusTypeDef init(const common::Fan& fan);
    void set_speed(const common::Fan& fan, const uint8_t speed_0_to_99);
namespace all {
    HAL_StatusTypeDef init();
    /**
     * Sets speed of all fans
     * @param speed_0_to_99 when 0 fan_fb 307 Hz, when 95 28 Hz, when greater than 95 fan stops
     */
    void set_speed(const uint8_t speed_0_to_99);
    void stop();
    void start_full_speed();
    void start_half_speed();
    void start_min_speed();
    void test_speed();
}
}
}
}