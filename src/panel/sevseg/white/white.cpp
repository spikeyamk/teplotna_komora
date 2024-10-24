#include <array>
#include <bitset>
#include <iostream>
#include <cmath>
#include "stm32f2xx_hal.h"
#include "cmsis_os.h"
#include "panel/sevseg/white/white.hpp"

#include "tim.h"

namespace panel {
namespace sevseg {
namespace white {
    void init_brightness() {
        HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    }

    void dim() {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 50'000);
    }

    void bright() {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 60'000);
    }

    void turn_on_all_segments() {
        //cathodes
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET); //4th digit
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);

        //segments
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);  //A
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);  //E
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);  //F
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET); //B
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET); //C
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET); //D
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET); //G
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET); //DP
    }

    const std::array<uint16_t, 8> active_segment {
        GPIO_PIN_13, //stred
        GPIO_PIN_9,  //vlavo hore
        GPIO_PIN_8,  //vlavo dole
        GPIO_PIN_12, //dole
        GPIO_PIN_11, //vpravo dole
        GPIO_PIN_10, //vpravo hore
        GPIO_PIN_7,  //hore
        GPIO_PIN_14  //decimal point
        };
    const std::array<uint16_t, 5> active_cathodes {
        GPIO_PIN_4,
        GPIO_PIN_3,
        GPIO_PIN_2, 
        GPIO_PIN_0,
        GPIO_PIN_1,
    };

    void display_worker(void *argument) {
        float& number { *reinterpret_cast<float*>(argument)};
        const auto sevmap {common::float_to_sevmap(number)};
        init_brightness();
        dim();
        while (1) {
            display_refresh(sevmap);
            osDelay(1);
        }
    }

    void launch_display_task(float& number) {
    static uint32_t sevw_buffer[2048];
    static StaticTask_t sevw_refresh;
    const osThreadAttr_t sevw_attr {
        .name = "sevw_display",
        .attr_bits = osThreadDetached,
        .cb_mem = &sevw_refresh,
        .cb_size = sizeof(sevw_refresh),
        .stack_mem = &sevw_buffer[0],
        .stack_size = sizeof(sevw_buffer),
        .priority = (osPriority_t) osPriorityNormal,
        .tz_module = 0,
        .reserved = 0,
    };
    osThreadNew(display_worker, reinterpret_cast<void*>(&number), &sevw_attr);
    }
}
}
}