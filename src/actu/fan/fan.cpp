#include <limits>
#include <iostream>
#include "tim.h"
#include "actu/fan/fan.hpp"

/*
TIM_HandleTypeDef htim5;
uint32_t capture_value1 = 0;
uint32_t capture_value2 = 0;
uint32_t capture_difference = 0;
uint32_t fan_rpm = 0;
volatile uint8_t capture_done = 0;
*/

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    /*
    std::printf("HAL_TIM_IC_CaptureCallback: %p\n\r", htim);
    if(htim->Instance == TIM5) {
        if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
            if (capture_done == 0) {
                // First capture event
                capture_value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
                capture_done = 1;
            } else if (capture_done == 1) {
                // Second capture event
                capture_value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
                if (capture_value2 > capture_value1) {
                    capture_difference = capture_value2 - capture_value1;
                } else {
                    capture_difference = (0xFFFFFFFF - capture_value1) + capture_value2;
                }
                capture_done = 0;
                
                // Calculate RPM (Assuming 2 pulses per revolution)
                fan_rpm = (60 * 1000000) / (capture_difference * 2);  // In RPM
            }
        }
    }
    */
}


namespace actu {
namespace fan {
    /**
     * @brief TIM5 Initialization Function
     * @param None
     * @retval None
     */
    
    /**
     * @brief  This function is executed in case of error occurrence.
     * @retval None
     */
    void Error_Handler(void) {
        /* USER CODE BEGIN Error_Handler_Debug */
        /* User can add his own implementation to report the HAL error return state */
        __disable_irq();
        while (1)
        {
        }
        /* USER CODE END Error_Handler_Debug */
    }

    void init_ctl() {
        HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
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

    void stop_all() {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, 65535);
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, 65535);
        __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, 65535);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 65535);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 65535);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 65535);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 65535);
        __HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, 65535);
    }

    void start_all() {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, 0);
    }

    void start_all_half_speed() {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, (2 << 14));
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, (2 << 14));
        __HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, (2 << 14));
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, (2 << 14));
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, (2 << 14));
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, (2 << 14));
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, (2 << 14));
        __HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, (2 << 14));
    }
}
}