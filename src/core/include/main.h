/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FAN_BR_CTL_Pin GPIO_PIN_0
#define FAN_BR_CTL_GPIO_Port GPIOA
#define FAN_BR_FB_Pin GPIO_PIN_1
#define FAN_BR_FB_GPIO_Port GPIOA
#define FAN_FR_CTL_Pin GPIO_PIN_2
#define FAN_FR_CTL_GPIO_Port GPIOA
#define FAN_FR_FB_Pin GPIO_PIN_3
#define FAN_FR_FB_GPIO_Port GPIOA
#define ENCA_EXTI_Pin GPIO_PIN_10
#define ENCA_EXTI_GPIO_Port GPIOB
#define ENCA_EXTI_EXTI_IRQn EXTI15_10_IRQn
#define ENCB_Pin GPIO_PIN_11
#define ENCB_GPIO_Port GPIOB
#define SPI2_NSS_Pin GPIO_PIN_12
#define SPI2_NSS_GPIO_Port GPIOB
#define FAN_RR_CTL_Pin GPIO_PIN_12
#define FAN_RR_CTL_GPIO_Port GPIOD
#define FAN_RR_FB_Pin GPIO_PIN_13
#define FAN_RR_FB_GPIO_Port GPIOD
#define FAN_BL_CTL_Pin GPIO_PIN_14
#define FAN_BL_CTL_GPIO_Port GPIOD
#define FAN_BL_CTLD15_Pin GPIO_PIN_15
#define FAN_BL_CTLD15_GPIO_Port GPIOD
#define FAN_RL_CTL_Pin GPIO_PIN_6
#define FAN_RL_CTL_GPIO_Port GPIOC
#define FAN_RL_FB_Pin GPIO_PIN_7
#define FAN_RL_FB_GPIO_Port GPIOC
#define FAN_FL_CTL_Pin GPIO_PIN_8
#define FAN_FL_CTL_GPIO_Port GPIOC
#define FAN_FL_FB_Pin GPIO_PIN_9
#define FAN_FL_FB_GPIO_Port GPIOC
#define SEVW_BKLT_Pin GPIO_PIN_15
#define SEVW_BKLT_GPIO_Port GPIOA
#define SPI3_DRDY0_Pin GPIO_PIN_4
#define SPI3_DRDY0_GPIO_Port GPIOD
#define SPI3_DRDY0_EXTI_IRQn EXTI4_IRQn
#define SPI3_NSS0_Pin GPIO_PIN_5
#define SPI3_NSS0_GPIO_Port GPIOD
#define SPI3_DRDY1_Pin GPIO_PIN_6
#define SPI3_DRDY1_GPIO_Port GPIOD
#define SPI3_DRDY1_EXTI_IRQn EXTI9_5_IRQn
#define SPI3_NSS1_Pin GPIO_PIN_7
#define SPI3_NSS1_GPIO_Port GPIOD
#define I2CBB0_SCL_Pin GPIO_PIN_6
#define I2CBB0_SCL_GPIO_Port GPIOB
#define I2CBB0_SDA_Pin GPIO_PIN_7
#define I2CBB0_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
