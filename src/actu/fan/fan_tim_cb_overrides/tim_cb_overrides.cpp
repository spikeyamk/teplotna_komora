#include <iostream>
#include "actu/fan/fb/fb.hpp"
#include "tim.h"

extern "C" void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    for(const auto& e: actu::fan::fb::fbs) {
        if(htim->Instance == e.get().fan.htim->Instance && htim->Channel == e.get().fan.fb_channel.active_channel_for_interrupt) {
            e.get().record_input_capture();
        }
    }
}

extern "C" void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
    for(const auto& e: actu::fan::fb::fbs) {
        if(htim->Instance == e.get().fan.htim->Instance && htim->Channel == e.get().fan.ctl_channel.active_channel_for_interrupt) {
            e.get().increment_pwm_pulse_finished_overflows();
        }
    }
}