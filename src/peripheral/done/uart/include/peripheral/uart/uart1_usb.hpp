#pragma once

#include "stm32f2xx_hal.h"

namespace peripheral {
namespace uart {
namespace uart1_usb {
    extern const GPIO_TypeDef* PORT;
    extern UART_HandleTypeDef huart1;

    enum class Pins {
        UART1_USB_TX = GPIO_PIN_9,
        UART1_USB_RX = GPIO_PIN_10,
    };

    void init();
    void deinit();
}
}
}