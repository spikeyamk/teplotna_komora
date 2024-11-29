#pragma once

#include <array>
#include <functional>
#include "stm32f2xx_hal.h"

namespace actu {
namespace fan {
namespace common {
    struct Fan {
        TIM_HandleTypeDef* htim;
        struct Channel {
            const uint32_t mask_for_init;
            const HAL_TIM_ActiveChannel active_channel_for_interrupt;
        };
        const Channel ctl_channel;
        const Channel fb_channel;
    };

    extern const Fan FAN0_REAR_LEFT;
    extern const Fan FAN1_FRONT_LEFT;
    extern const Fan FAN2_REAR_RIGHT;
    extern const Fan FAN3_BOTTOM_LEFT;
    extern const Fan FAN4_BOTTOM_RIGHT;
    extern const Fan FAN5_FRONT_RIGHT;
    extern const std::array<Fan, 6> fans;
}
}
}