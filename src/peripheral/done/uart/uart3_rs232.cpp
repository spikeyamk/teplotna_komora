#include <iostream>
#include "peripheral/uart/uart3_rs232.hpp"

namespace peripheral {
namespace uart {
namespace uart3_rs232 {
    const GPIO_TypeDef* PORT { GPIOD };
    UART_HandleTypeDef huart3 {};

    void Error_Handler() {
        std::cout << "peripheral::uart3_rs232::Error_Handler\n";
    }

    void init() {
        huart3.Instance = USART3;
        huart3.Init.BaudRate = 115200;
        huart3.Init.WordLength = UART_WORDLENGTH_8B;
        huart3.Init.StopBits = UART_STOPBITS_1;
        huart3.Init.Parity = UART_PARITY_NONE;
        huart3.Init.Mode = UART_MODE_TX_RX;
        huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart3.Init.OverSampling = UART_OVERSAMPLING_16;
        if(HAL_UART_Init(&huart3) != HAL_OK) {
            Error_Handler();
        }
    }

    void deinit() {
        if(HAL_UART_DeInit(&huart3) != HAL_OK) {
            Error_Handler();
        }
    }
}
}
}