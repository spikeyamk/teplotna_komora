#include <limits>
#include <trielo/trielo.hpp>
#include "tim.h"
#include "actu/fan/fan.hpp"

namespace actu {
namespace fan {
    void init_ctl() {
        HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    }

    void init_fb() {
        HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_2);
        /*
        HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_4);
        HAL_TIM_IC_Start_IT(htim11, TIM_CHANNEL_1);
        HAL_TIM_IC_Start_IT(htim3, TIM_CHANNEL_2);
        HAL_TIM_IC_Start_IT(htim3, TIM_CHANNEL_4);
        HAL_TIM_IC_Start_IT(htim4, TIM_CHANNEL_2);
        HAL_TIM_IC_Start_IT(htim4, TIM_CHANNEL_4);
        HAL_TIM_IC_Start_IT(htim9, TIM_CHANNEL_2);
        */
    }

    void test_speed() {
        for(uint8_t i = 0; true; i = (i > 99 ? 0 : (i + 10))) {
            Trielo::trielo<set_speed>(i);
            HAL_Delay(5'000);
        }
    }

    void set_speed(const uint8_t speed_0_to_100) {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, speed_0_to_100);
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, speed_0_to_100);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed_0_to_100);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed_0_to_100);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, speed_0_to_100);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, speed_0_to_100);
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

    void start_min_speed() {
        set_speed(95);
    }
}
}