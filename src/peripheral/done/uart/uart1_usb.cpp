#include <iostream>
#include "peripheral/uart/uart1_usb.hpp"

namespace peripheral {
namespace uart {
namespace uart1_usb {
    const GPIO_TypeDef* PORT { GPIOA };
    extern UART_HandleTypeDef huart1;

    void Error_Handler() {
        std::cout << "peripheral::uart::uart1_usb::Error_Handler\n";
    }

    void init() {
        huart1.Instance = USART1;
        huart1.Init.BaudRate = 115200;
        huart1.Init.WordLength = UART_WORDLENGTH_8B;
        huart1.Init.StopBits = UART_STOPBITS_1;
        huart1.Init.Parity = UART_PARITY_NONE;
        huart1.Init.Mode = UART_MODE_TX_RX;
        huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart1.Init.OverSampling = UART_OVERSAMPLING_16;
        if(HAL_UART_Init(&huart1) != HAL_OK) {
            Error_Handler();
        }
    }

    void deinit() {
        if(HAL_UART_DeInit(&huart1) != HAL_OK) {
            Error_Handler();
        }
    }
}
}
}