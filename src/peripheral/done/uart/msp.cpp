#include "peripheral/uart/uart1_usb.hpp"
#include "peripheral/uart/uart3_rs232.hpp"

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle) {
    if(uartHandle->Instance==USART1) {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        using namespace peripheral::uart::uart1_usb;
        const GPIO_InitTypeDef GPIO_InitStruct {
            .Pin =
                static_cast<uint16_t>(Pins::UART1_USB_RX)
                | static_cast<uint16_t>(Pins::UART1_USB_TX),
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
            .Alternate = GPIO_AF7_USART1,
        };
        HAL_GPIO_Init(const_cast<GPIO_TypeDef*>(PORT), const_cast<GPIO_InitTypeDef*>(&GPIO_InitStruct));
    } else if(uartHandle->Instance==USART3) {
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        using namespace peripheral::uart::uart3_rs232;
        const GPIO_InitTypeDef GPIO_InitStruct {
            .Pin =
                static_cast<uint16_t>(Pins::UART3_RS232_RX)
                | static_cast<uint16_t>(Pins::UART3_RS232_TX),
            .Mode = GPIO_MODE_AF_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
            .Alternate = GPIO_AF7_USART3,
        };
        HAL_GPIO_Init(const_cast<GPIO_TypeDef*>(PORT), const_cast<GPIO_InitTypeDef*>(&GPIO_InitStruct));
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle) {
    if(uartHandle->Instance==USART1) {
        __HAL_RCC_USART1_CLK_DISABLE();
        using namespace peripheral::uart::uart1_usb;
        HAL_GPIO_DeInit(
            const_cast<GPIO_TypeDef*>(PORT),
            static_cast<uint16_t>(Pins::UART1_USB_RX)
            | static_cast<uint16_t>(Pins::UART1_USB_TX)
        );
    } else if(uartHandle->Instance==USART3) {
        __HAL_RCC_USART3_CLK_DISABLE();
        using namespace peripheral::uart::uart3_rs232;
        HAL_GPIO_DeInit(
            const_cast<GPIO_TypeDef*>(PORT),
             static_cast<uint16_t>(Pins::UART3_RS232_RX)
            | static_cast<uint16_t>(Pins::UART3_RS232_TX)
        );
    }
}