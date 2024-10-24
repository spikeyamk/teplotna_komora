#include <limits>
#include <trielo/trielo.hpp>
#include "tim.h"
#include "actu/fan/ctl/ctl.hpp"

namespace actu {
namespace fan {
namespace ctl {
    void init() {
        HAL_TIM_PWM_Start_IT(&htim5, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start_IT(&htim5, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_3);
    }

    void test_speed() {
        for(uint8_t i = 0; true; i = (i > 99 ? 0 : (i + 10))) {
            Trielo::trielo<set_speed>(i);
            HAL_Delay(5'000);
        }
    }

    void set_speed(const uint8_t speed_0_to_99) {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, speed_0_to_99);
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, speed_0_to_99);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed_0_to_99);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed_0_to_99);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, speed_0_to_99);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, speed_0_to_99);
    }

    void stop_all() {
        set_speed(99);
    }

    void start_all() {
        set_speed(0);
    }

    void start_all_half_speed() {
        set_speed(50);
    }

    void start_all_min_speed() {
        set_speed(95);
    }
}
}
}