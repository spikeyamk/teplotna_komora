#include <trielo/trielo.hpp>
#include "actu/fan/fb/fb.hpp"

namespace actu {
namespace fan {
namespace fb {
    FB FAN0_REAR_LEFT    { common::FAN0_REAR_LEFT    };
    FB FAN1_FRONT_LEFT   { common::FAN1_FRONT_LEFT   };
    FB FAN2_REAR_RIGHT   { common::FAN2_REAR_RIGHT   };
    FB FAN3_BOTTOM_LEFT  { common::FAN3_BOTTOM_LEFT  };
    FB FAN4_BOTTOM_RIGHT { common::FAN4_BOTTOM_RIGHT };
    FB FAN5_FRONT_RIGHT  { common::FAN5_FRONT_RIGHT  };

    const std::array<std::reference_wrapper<FB>, 6> fbs {
		std::ref(FAN0_REAR_LEFT),
		std::ref(FAN1_FRONT_LEFT),
		std::ref(FAN2_REAR_RIGHT),
		std::ref(FAN3_BOTTOM_LEFT),
		std::ref(FAN4_BOTTOM_RIGHT),
		std::ref(FAN5_FRONT_RIGHT),
    };

    HAL_StatusTypeDef init(const common::Fan& fan) {
        return HAL_TIM_IC_Start_IT(fan.htim, fan.fb_channel.mask_for_init);
    }

    void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
        for(const auto& e: fbs) {
            if(htim->Instance == e.get().fan.htim->Instance && htim->Channel == e.get().fan.fb_channel.active_channel_for_interrupt) {
                e.get().record_input_capture();
            }
        }
    }

    void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
        for(const auto& e: fbs) {
            if(htim->Instance == e.get().fan.htim->Instance && htim->Channel == e.get().fan.ctl_channel.active_channel_for_interrupt) {
                e.get().increment_pwm_pulse_finished_overflows();
            }
        }
    }
namespace all {
    HAL_StatusTypeDef init() {
        for(const auto& e: common::fans) {
            const HAL_StatusTypeDef err_ret { ::actu::fan::fb::init(e) };
            if(err_ret != HAL_OK) {
                return err_ret;
            }
        }
        return HAL_OK;
    }
}
}
}
}