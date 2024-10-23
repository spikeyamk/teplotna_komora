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
#define SEVW_CC2_Pin GPIO_PIN_2
#define SEVW_CC2_GPIO_Port GPIOE
#define SEVW_CC3_Pin GPIO_PIN_3
#define SEVW_CC3_GPIO_Port GPIOE
#define SEVW_CC4_Pin GPIO_PIN_4
#define SEVW_CC4_GPIO_Port GPIOE
#define LED0_COOL_Pin GPIO_PIN_0
#define LED0_COOL_GPIO_Port GPIOC
#define LED1_HEAT_Pin GPIO_PIN_1
#define LED1_HEAT_GPIO_Port GPIOC
#define LED2_RS232_Pin GPIO_PIN_2
#define LED2_RS232_GPIO_Port GPIOC
#define LED3_USB_Pin GPIO_PIN_3
#define LED3_USB_GPIO_Port GPIOC
#define TIM5_CH1_FAN4_BR_CTL_Pin GPIO_PIN_0
#define TIM5_CH1_FAN4_BR_CTL_GPIO_Port GPIOA
#define TIM5_CH2_FAN4_BR_FB_Pin GPIO_PIN_1
#define TIM5_CH2_FAN4_BR_FB_GPIO_Port GPIOA
#define TIM5_CH3_FAN5_FR_CTL_Pin GPIO_PIN_2
#define TIM5_CH3_FAN5_FR_CTL_GPIO_Port GPIOA
#define TIM5_CH4_FAN5_FR_FB_Pin GPIO_PIN_3
#define TIM5_CH4_FAN5_FR_FB_GPIO_Port GPIOA
#define DAC_OUT1_LSF_Pin GPIO_PIN_4
#define DAC_OUT1_LSF_GPIO_Port GPIOA
#define DAC_OUT2_LSR_Pin GPIO_PIN_5
#define DAC_OUT2_LSR_GPIO_Port GPIOA
#define BRDGF_LHIGH_Pin GPIO_PIN_4
#define BRDGF_LHIGH_GPIO_Port GPIOC
#define BRDGF_RHIGH_Pin GPIO_PIN_5
#define BRDGF_RHIGH_GPIO_Port GPIOC
#define BRDGF_LLOW_Pin GPIO_PIN_0
#define BRDGF_LLOW_GPIO_Port GPIOB
#define BRDGF_RLOW_Pin GPIO_PIN_1
#define BRDGF_RLOW_GPIO_Port GPIOB
#define SEVW_SA_Pin GPIO_PIN_7
#define SEVW_SA_GPIO_Port GPIOE
#define SEVW_SB_Pin GPIO_PIN_8
#define SEVW_SB_GPIO_Port GPIOE
#define SEVW_SC_Pin GPIO_PIN_9
#define SEVW_SC_GPIO_Port GPIOE
#define SEVW_SD_Pin GPIO_PIN_10
#define SEVW_SD_GPIO_Port GPIOE
#define SEVW_SE_Pin GPIO_PIN_11
#define SEVW_SE_GPIO_Port GPIOE
#define SEVW_SF_Pin GPIO_PIN_12
#define SEVW_SF_GPIO_Port GPIOE
#define SEVW_SG_Pin GPIO_PIN_13
#define SEVW_SG_GPIO_Port GPIOE
#define SEVW_SDP_Pin GPIO_PIN_14
#define SEVW_SDP_GPIO_Port GPIOE
#define ENCA_EXTI10_Pin GPIO_PIN_10
#define ENCA_EXTI10_GPIO_Port GPIOB
#define ENCA_EXTI10_EXTI_IRQn EXTI15_10_IRQn
#define ENCB_Pin GPIO_PIN_11
#define ENCB_GPIO_Port GPIOB
#define SPI2_SEVYG_NSS_Pin GPIO_PIN_12
#define SPI2_SEVYG_NSS_GPIO_Port GPIOB
#define SPI2_SEVYG_SCK_Pin GPIO_PIN_13
#define SPI2_SEVYG_SCK_GPIO_Port GPIOB
#define SPI2_SEVYG_MISO_Pin GPIO_PIN_14
#define SPI2_SEVYG_MISO_GPIO_Port GPIOB
#define SPI2_SEVYG_MOSI_Pin GPIO_PIN_15
#define SPI2_SEVYG_MOSI_GPIO_Port GPIOB
#define RS232_UART_TX_Pin GPIO_PIN_8
#define RS232_UART_TX_GPIO_Port GPIOD
#define RS232_UART_RX_Pin GPIO_PIN_9
#define RS232_UART_RX_GPIO_Port GPIOD
#define BUZZEN_Pin GPIO_PIN_10
#define BUZZEN_GPIO_Port GPIOD
#define NPUMPEN_Pin GPIO_PIN_11
#define NPUMPEN_GPIO_Port GPIOD
#define TIM4_CH1_FAN2_RR_CTL_Pin GPIO_PIN_12
#define TIM4_CH1_FAN2_RR_CTL_GPIO_Port GPIOD
#define TIM4_CH2_FAN2_RR_FB_Pin GPIO_PIN_13
#define TIM4_CH2_FAN2_RR_FB_GPIO_Port GPIOD
#define TIM4_CH3_FAN3_BL_CTL_Pin GPIO_PIN_14
#define TIM4_CH3_FAN3_BL_CTL_GPIO_Port GPIOD
#define TIM4_CH4_FAN3_BL_FB_Pin GPIO_PIN_15
#define TIM4_CH4_FAN3_BL_FB_GPIO_Port GPIOD
#define TIM3_CH1_FAN0_RL_CTL_Pin GPIO_PIN_6
#define TIM3_CH1_FAN0_RL_CTL_GPIO_Port GPIOC
#define TIM3_CH2_FAN0_RL_FB_Pin GPIO_PIN_7
#define TIM3_CH2_FAN0_RL_FB_GPIO_Port GPIOC
#define TIM3_CH3_FAN1_FL_CTL_Pin GPIO_PIN_8
#define TIM3_CH3_FAN1_FL_CTL_GPIO_Port GPIOC
#define TIM3_CH4_FAN1_FL_FB_Pin GPIO_PIN_9
#define TIM3_CH4_FAN1_FL_FB_GPIO_Port GPIOC
#define BRDGR_RLOW_Pin GPIO_PIN_8
#define BRDGR_RLOW_GPIO_Port GPIOA
#define USB_UART_TX_Pin GPIO_PIN_9
#define USB_UART_TX_GPIO_Port GPIOA
#define USB_UART_RX_Pin GPIO_PIN_10
#define USB_UART_RX_GPIO_Port GPIOA
#define TIM2_CH1_SEVW_BKLT_Pin GPIO_PIN_15
#define TIM2_CH1_SEVW_BKLT_GPIO_Port GPIOA
#define BRDGR_LLOW_Pin GPIO_PIN_10
#define BRDGR_LLOW_GPIO_Port GPIOC
#define BRDGR_RHIGH_Pin GPIO_PIN_11
#define BRDGR_RHIGH_GPIO_Port GPIOC
#define BRDGR_LHIGH_Pin GPIO_PIN_12
#define BRDGR_LHIGH_GPIO_Port GPIOC
#define BUT0_FR_Pin GPIO_PIN_0
#define BUT0_FR_GPIO_Port GPIOD
#define BUT0_FR_EXTI_IRQn EXTI0_IRQn
#define BUT1_MR_Pin GPIO_PIN_1
#define BUT1_MR_GPIO_Port GPIOD
#define BUT1_MR_EXTI_IRQn EXTI1_IRQn
#define BUT2_ML_Pin GPIO_PIN_2
#define BUT2_ML_GPIO_Port GPIOD
#define BUT2_ML_EXTI_IRQn EXTI2_IRQn
#define BUT3_FL_Pin GPIO_PIN_3
#define BUT3_FL_GPIO_Port GPIOD
#define BUT3_FL_EXTI_IRQn EXTI3_IRQn
#define SPI3_TEMP_NDRDY0_Pin GPIO_PIN_4
#define SPI3_TEMP_NDRDY0_GPIO_Port GPIOD
#define SPI3_TEMP_NDRDY0_EXTI_IRQn EXTI4_IRQn
#define SPI3_TEMP_NSS0_Pin GPIO_PIN_5
#define SPI3_TEMP_NSS0_GPIO_Port GPIOD
#define SPI3_TEMP_NDRDY1_Pin GPIO_PIN_6
#define SPI3_TEMP_NDRDY1_GPIO_Port GPIOD
#define SPI3_TEMP_NDRDY1_EXTI_IRQn EXTI9_5_IRQn
#define SPI3_TEMP_NSS1_Pin GPIO_PIN_7
#define SPI3_TEMP_NSS1_GPIO_Port GPIOD
#define SPI3_TEMP_SCK_Pin GPIO_PIN_3
#define SPI3_TEMP_SCK_GPIO_Port GPIOB
#define SPI3_TEMP_MISO_Pin GPIO_PIN_4
#define SPI3_TEMP_MISO_GPIO_Port GPIOB
#define SPI3_TEMP_MOSI_Pin GPIO_PIN_5
#define SPI3_TEMP_MOSI_GPIO_Port GPIOB
#define I2CBB1_SCL_Pin GPIO_PIN_6
#define I2CBB1_SCL_GPIO_Port GPIOB
#define I2CBB1_SDA_Pin GPIO_PIN_7
#define I2CBB1_SDA_GPIO_Port GPIOB
#define SEVW_CC0_Pin GPIO_PIN_0
#define SEVW_CC0_GPIO_Port GPIOE
#define SEVW_CC1_Pin GPIO_PIN_1
#define SEVW_CC1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
