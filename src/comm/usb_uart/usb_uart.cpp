#include "stm32h7xx_nucleo.h"
#include "comm/usb_uart/usb_uart.hpp"

extern "C" void Error_Handler(void);

namespace comm {
namespace usb_uart {
    const uint16_t STLK_VCP_RX_Pin { GPIO_PIN_8 };
    const GPIO_TypeDef* STLK_VCP_RX_GPIO_Port { GPIOD };
    const uint16_t STLK_VCP_TX_Pin { GPIO_PIN_9 };
    const GPIO_TypeDef* STLK_VCP_TX_GPIO_Port { GPIOD };

    UART_HandleTypeDef huart3;

    void MX_USART3_UART_Init(void) {

        /* USER CODE BEGIN USART3_Init 0 */

        /* USER CODE END USART3_Init 0 */

        /* USER CODE BEGIN USART3_Init 1 */

        /* USER CODE END USART3_Init 1 */
        huart3.Instance = USART3;
        huart3.Init.BaudRate = 115200;
        huart3.Init.WordLength = UART_WORDLENGTH_8B;
        huart3.Init.StopBits = UART_STOPBITS_1;
        huart3.Init.Parity = UART_PARITY_NONE;
        huart3.Init.Mode = UART_MODE_TX_RX;
        huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart3.Init.OverSampling = UART_OVERSAMPLING_16;
        huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
        huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
        huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
        if (HAL_UART_Init(&huart3) != HAL_OK)
        {
            Error_Handler();
        }
        if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
        {
            Error_Handler();
        }
        if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
        {
            Error_Handler();
        }
        if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
        {
            Error_Handler();
        }
        /* USER CODE BEGIN USART3_Init 2 */

        /* USER CODE END USART3_Init 2 */

    }

    void init() {
        /* USER CODE BEGIN 2 */
        MX_USART3_UART_Init();
        /* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
        COM_InitTypeDef BspCOMInit;
        BspCOMInit.BaudRate   = 115200;
        BspCOMInit.WordLength = COM_WORDLENGTH_8B;
        BspCOMInit.StopBits   = COM_STOPBITS_1;
        BspCOMInit.Parity     = COM_PARITY_NONE;
        BspCOMInit.HwFlowCtl  = COM_HWCONTROL_NONE;
        if(BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE) {
            Error_Handler();
        }
    }

    void deinit() {
        HAL_UART_DeInit(&huart3);
    }

    void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle) {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
        if(uartHandle->Instance==USART3) {
        /* USER CODE BEGIN USART3_MspInit 0 */

        /* USER CODE END USART3_MspInit 0 */

        /** Initializes the peripherals clock
         */
            PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
            PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
            if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
                Error_Handler();
            }

            /* USART3 clock enable */
            __HAL_RCC_USART3_CLK_ENABLE();

            __HAL_RCC_GPIOD_CLK_ENABLE();
            /**USART3 GPIO Configuration
            PD8     ------> USART3_TX
            PD9     ------> USART3_RX
            */
            GPIO_InitStruct.Pin = STLK_VCP_RX_Pin|STLK_VCP_TX_Pin;
            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
            HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* USER CODE BEGIN USART3_MspInit 1 */

        /* USER CODE END USART3_MspInit 1 */
        }
    }

    void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle) {
        if(uartHandle->Instance==USART3) {
        /* USER CODE BEGIN USART3_MspDeInit 0 */

        /* USER CODE END USART3_MspDeInit 0 */
            /* Peripheral clock disable */
            __HAL_RCC_USART3_CLK_DISABLE();

            /**USART3 GPIO Configuration
            PD8     ------> USART3_TX
            PD9     ------> USART3_RX
            */
            HAL_GPIO_DeInit(GPIOD, STLK_VCP_RX_Pin|STLK_VCP_TX_Pin);

        /* USER CODE BEGIN USART3_MspDeInit 1 */

        /* USER CODE END USART3_MspDeInit 1 */
        }
    }
}
}