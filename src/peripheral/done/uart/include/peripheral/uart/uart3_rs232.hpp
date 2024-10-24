#pragma once

#include "stm32f2xx_hal.h"

namespace peripheral {
namespace uart {
namespace uart3_rs232 {
    extern UART_HandleTypeDef huart3;
    extern const GPIO_TypeDef* PORT;

    enum class Pins {
        UART3_RS232_TX = GPIO_PIN_8,
        UART3_RS232_RX = GPIO_PIN_9,
    };

    void init();
    void deinit();
}
}
}