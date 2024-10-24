#include <iostream>
#include "actu/fan/fb/fb.hpp"
#include "tim.h"

extern "C" void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM5) {
        if(actu::fan::fb::overflows::tim5 % 100 == 0) {
            std::printf("TIM5_IC: %u\n", actu::fan::fb::overflows::tim5);
        }
        actu::fan::fb::overflows::tim5 = 0;
    } else if(htim->Instance == TIM4) {
        if(actu::fan::fb::overflows::tim4 % 100 == 0) {
            std::printf("TIM4_IC: %u\n", actu::fan::fb::overflows::tim4);
        }
        actu::fan::fb::overflows::tim4 = 0;
    } else if(htim->Instance == TIM3) {
        if(actu::fan::fb::overflows::tim3 % 100 == 0) {
            std::printf("TIM3_IC: %u\n", actu::fan::fb::overflows::tim3);
        }
        actu::fan::fb::overflows::tim3 = 0;
    }
}

extern "C" void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM5) {
        actu::fan::fb::overflows::tim5++;
    } else if(htim->Instance == TIM4) {
        actu::fan::fb::overflows::tim4++;
    } else if(htim->Instance == TIM3) {
        actu::fan::fb::overflows::tim3++;
    }
}