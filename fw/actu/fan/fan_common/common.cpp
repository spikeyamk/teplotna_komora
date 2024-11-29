#include <limits>
#include <trielo/trielo.hpp>
#include "cmsis_os2.h"
#include "tim.h"
#include "actu/fan/common/common.hpp"

namespace actu {
namespace fan {
namespace common {
    const Fan FAN0_REAR_LEFT {
        .htim = &htim3,
        .ctl_channel = {
            .mask_for_init = TIM_CHANNEL_1,
            .active_channel_for_interrupt = HAL_TIM_ACTIVE_CHANNEL_1,
        },
        .fb_channel = {
            .mask_for_init = TIM_CHANNEL_2,
            .active_channel_for_interrupt = HAL_TIM_ACTIVE_CHANNEL_2,
        },
    };

    const Fan FAN1_FRONT_LEFT {
        .htim = &htim3,
        .ctl_channel = {
            .mask_for_init = TIM_CHANNEL_3,
            .active_channel_for_interrupt = HAL_TIM_ACTIVE_CHANNEL_3,
        },
        .fb_channel = {
            .mask_for_init = TIM_CHANNEL_4,
            .active_channel_for_interrupt = HAL_TIM_ACTIVE_CHANNEL_4,
        },
    };

    const Fan FAN2_REAR_RIGHT {
        .htim = &htim4,
        .ctl_channel = {
            .mask_for_init = TIM_CHANNEL_1,
            .active_channel_for_interrupt = HAL_TIM_ACTIVE_CHANNEL_1,
        },
        .fb_channel = {
            .mask_for_init = TIM_CHANNEL_2,
            .active_channel_for_interrupt = HAL_TIM_ACTIVE_CHANNEL_2,
        },
    };

    const Fan FAN3_BOTTOM_LEFT {
        .htim = &htim4,
        .ctl_channel = {
            .mask_for_init = TIM_CHANNEL_3,
            .active_channel_for_interrupt = HAL_TIM_ACTIVE_CHANNEL_3,
        },
        .fb_channel = {
            .mask_for_init = TIM_CHANNEL_4,
            .active_channel_for_interrupt = HAL_TIM_ACTIVE_CHANNEL_4,
        },
    };

    const Fan FAN4_BOTTOM_RIGHT {
        .htim = &htim5,
        .ctl_channel = {
            .mask_for_init = TIM_CHANNEL_1,
            .active_channel_for_interrupt = HAL_TIM_ACTIVE_CHANNEL_1,
        },
        .fb_channel = {
            .mask_for_init = TIM_CHANNEL_2,
            .active_channel_for_interrupt = HAL_TIM_ACTIVE_CHANNEL_2,
        },
    };

    const Fan FAN5_FRONT_RIGHT {
        .htim = &htim5,
        .ctl_channel = {
            .mask_for_init = TIM_CHANNEL_3,
            .active_channel_for_interrupt = HAL_TIM_ACTIVE_CHANNEL_3,
        },
        .fb_channel = {
            .mask_for_init = TIM_CHANNEL_4,
            .active_channel_for_interrupt = HAL_TIM_ACTIVE_CHANNEL_4,
        },
    };

    const std::array<Fan, 6> fans {
        FAN0_REAR_LEFT,
        FAN1_FRONT_LEFT,
        FAN2_REAR_RIGHT,
        FAN3_BOTTOM_LEFT,
        FAN4_BOTTOM_RIGHT,
        FAN5_FRONT_RIGHT,
    };
}
}
}