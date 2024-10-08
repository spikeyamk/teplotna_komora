#include <limits>
#include <iostream>
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
    
    TIM_HandleTypeDef htim5;

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

    void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim) {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        if(htim->Instance==TIM2)
        {
        /* USER CODE BEGIN TIM2_MspPostInit 0 */

        /* USER CODE END TIM2_MspPostInit 0 */
            __HAL_RCC_GPIOA_CLK_ENABLE();
            /**TIM2 GPIO Configuration
            PA15     ------> TIM2_CH1
            */
            GPIO_InitStruct.Pin = GPIO_PIN_15;
            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
            HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM2_MspPostInit 1 */

        /* USER CODE END TIM2_MspPostInit 1 */
        }
        else if(htim->Instance==TIM3)
        {
        /* USER CODE BEGIN TIM3_MspPostInit 0 */

        /* USER CODE END TIM3_MspPostInit 0 */

            __HAL_RCC_GPIOC_CLK_ENABLE();
            /**TIM3 GPIO Configuration
            PC6     ------> TIM3_CH1
            PC8     ------> TIM3_CH3
            */
            GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_8;
            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
            HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM3_MspPostInit 1 */

        /* USER CODE END TIM3_MspPostInit 1 */
        }
        else if(htim->Instance==TIM4)
        {
        /* USER CODE BEGIN TIM4_MspPostInit 0 */

        /* USER CODE END TIM4_MspPostInit 0 */

            __HAL_RCC_GPIOD_CLK_ENABLE();
            /**TIM4 GPIO Configuration
            PD12     ------> TIM4_CH1
            PD14     ------> TIM4_CH3
            */
            GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_14;
            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
            HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM4_MspPostInit 1 */

        /* USER CODE END TIM4_MspPostInit 1 */
        }
        else if(htim->Instance==TIM5)
        {
        /* USER CODE BEGIN TIM5_MspPostInit 0 */

        /* USER CODE END TIM5_MspPostInit 0 */

            __HAL_RCC_GPIOA_CLK_ENABLE();
            /**TIM5 GPIO Configuration
            PA0-WKUP     ------> TIM5_CH1
            PA2     ------> TIM5_CH3
            */
            GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2;
            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
            HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM5_MspPostInit 1 */

        /* USER CODE END TIM5_MspPostInit 1 */
        }
        else if(htim->Instance==TIM9)
        {
        /* USER CODE BEGIN TIM9_MspPostInit 0 */

        /* USER CODE END TIM9_MspPostInit 0 */

            __HAL_RCC_GPIOE_CLK_ENABLE();
            /**TIM9 GPIO Configuration
            PE5     ------> TIM9_CH1
            */
            GPIO_InitStruct.Pin = GPIO_PIN_5;
            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            GPIO_InitStruct.Alternate = GPIO_AF3_TIM9;
            HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM9_MspPostInit 1 */

        /* USER CODE END TIM9_MspPostInit 1 */
        }
        else if(htim->Instance==TIM10)
        {
        /* USER CODE BEGIN TIM10_MspPostInit 0 */

        /* USER CODE END TIM10_MspPostInit 0 */

            __HAL_RCC_GPIOB_CLK_ENABLE();
            /**TIM10 GPIO Configuration
            PB8     ------> TIM10_CH1
            */
            GPIO_InitStruct.Pin = GPIO_PIN_8;
            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            GPIO_InitStruct.Alternate = GPIO_AF3_TIM10;
            HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USER CODE BEGIN TIM10_MspPostInit 1 */

        /* USER CODE END TIM10_MspPostInit 1 */
        }
    }

    void MX_TIM5_Init(void) {
        /* USER CODE BEGIN TIM5_Init 0 */

        /* USER CODE END TIM5_Init 0 */

        TIM_MasterConfigTypeDef sMasterConfig = {0};
        TIM_OC_InitTypeDef sConfigOC = {0};
        TIM_IC_InitTypeDef sConfigIC = {0};

        /* USER CODE BEGIN TIM5_Init 1 */

        /* USER CODE END TIM5_Init 1 */
        htim5.Instance = TIM5;
        htim5.Init.Prescaler = 0;
        htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim5.Init.Period = 65535;
        htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
        if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
        {
            Error_Handler();
        }
        if (HAL_TIM_IC_Init(&htim5) != HAL_OK)
        {
            Error_Handler();
        }
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
        {
            Error_Handler();
        }
        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 65535;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
        {
            Error_Handler();
        }
        sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
        sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
        sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
        sConfigIC.ICFilter = 0;
        if (HAL_TIM_IC_ConfigChannel(&htim5, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
        {
            Error_Handler();
        }
        if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
        {
            Error_Handler();
        }
        /* USER CODE BEGIN TIM5_Init 2 */

        /* USER CODE END TIM5_Init 2 */
        HAL_TIM_MspPostInit(&htim5);
    }

    void init_tim() {
        MX_TIM5_Init();
    }

    void init_ctl(
        TIM_HandleTypeDef* htim10,
        TIM_HandleTypeDef* htim3,
        TIM_HandleTypeDef* htim4,
        TIM_HandleTypeDef* htim9
    ) {
        HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start(htim10, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(htim3, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(htim3, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start(htim4, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(htim4, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start(htim9, TIM_CHANNEL_1);
    }

    void init_fb(
        TIM_HandleTypeDef* htim11,
        TIM_HandleTypeDef* htim3,
        TIM_HandleTypeDef* htim4,
        TIM_HandleTypeDef* htim9
    ) {
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

    void stop_all(
        TIM_HandleTypeDef* htim10,
        TIM_HandleTypeDef* htim3,
        TIM_HandleTypeDef* htim4,
        TIM_HandleTypeDef* htim9
    ) {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, 65535);
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, 65535);
        __HAL_TIM_SET_COMPARE(htim10, TIM_CHANNEL_1, 65535);
        __HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_1, 65535);
        __HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_3, 65535);
        __HAL_TIM_SET_COMPARE(htim4, TIM_CHANNEL_1, 65535);
        __HAL_TIM_SET_COMPARE(htim4, TIM_CHANNEL_3, 65535);
        __HAL_TIM_SET_COMPARE(htim9, TIM_CHANNEL_1, 65535);
    }

    void start_all(
        TIM_HandleTypeDef* htim10,
        TIM_HandleTypeDef* htim3,
        TIM_HandleTypeDef* htim4,
        TIM_HandleTypeDef* htim9
    ) {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(htim10, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(htim4, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(htim4, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(htim9, TIM_CHANNEL_1, 0);
    }

    void start_all_half_speed(
        TIM_HandleTypeDef* htim10,
        TIM_HandleTypeDef* htim3,
        TIM_HandleTypeDef* htim4,
        TIM_HandleTypeDef* htim9
    ) {
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, (2 << 14));
        __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, (2 << 14));
        __HAL_TIM_SET_COMPARE(htim10, TIM_CHANNEL_1, (2 << 14));
        __HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_1, (2 << 14));
        __HAL_TIM_SET_COMPARE(htim3, TIM_CHANNEL_3, (2 << 14));
        __HAL_TIM_SET_COMPARE(htim4, TIM_CHANNEL_1, (2 << 14));
        __HAL_TIM_SET_COMPARE(htim4, TIM_CHANNEL_3, (2 << 14));
        __HAL_TIM_SET_COMPARE(htim9, TIM_CHANNEL_1, (2 << 14));
    }
}
}