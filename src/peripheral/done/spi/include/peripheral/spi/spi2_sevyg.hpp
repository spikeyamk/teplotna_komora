#pragma once

#include "stm32f2xx_hal.h"

namespace peripheral {
namespace spi {
namespace spi2_sevyg {
    extern SPI_HandleTypeDef hspi2;
    extern const GPIO_TypeDef* PORT;

    enum class Pins {
        SPI2_SEVYG_NSS = GPIO_PIN_12,
        SPI2_SEVYG_SCK = GPIO_PIN_13,
        SPI2_SEVYG_MISO = GPIO_PIN_14,
        SPI2_SEVYG_MOSI = GPIO_PIN_15,
    };

    void init();
    void deinit();
}
}
}