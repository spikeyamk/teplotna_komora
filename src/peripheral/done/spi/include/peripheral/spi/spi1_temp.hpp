#pragma once

#include "stm32f2xx_hal.h"

namespace peripheral {
namespace spi {
namespace spi1_temp {
    extern SPI_HandleTypeDef hspi1;

    namespace Ports {
        extern const GPIO_TypeDef* MOSI_MISO_SCK;
        extern const GPIO_TypeDef* SS0_SS1;
    };

    enum class Pins {
        SPI1_TEMP_SS0 = GPIO_PIN_5,
        SPI1_TEMP_SS1 = GPIO_PIN_7,
        SPI1_TEMP_SCK = GPIO_PIN_3,
        SPI1_TEMP_MISO = GPIO_PIN_4,
        SPI1_TEMP_MOSI = GPIO_PIN_5,
    };

    void init();
    void deinit();
}
}
}