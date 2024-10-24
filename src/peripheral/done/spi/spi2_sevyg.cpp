#include <iostream>
#include "peripheral/spi/spi2_sevyg.hpp"

namespace peripheral {
namespace spi {
namespace spi2_sevyg {
    const GPIO_TypeDef* PORT { GPIOB };

    void Error_Handler() {
        std::cout << "peripheral::spi::spi2_sevyg::Error_Handler\n";
    }

    void init() {
        hspi2.Instance = SPI2;
        hspi2.Init.Mode = SPI_MODE_MASTER;
        hspi2.Init.Direction = SPI_DIRECTION_2LINES;
        hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
        hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
        hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
        hspi2.Init.NSS = SPI_NSS_HARD_OUTPUT;
        hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
        hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
        hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
        hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        hspi2.Init.CRCPolynomial = 10;
        if(HAL_SPI_Init(&hspi2) != HAL_OK) {
            Error_Handler();
        }
    }

    void deinit() {
        if(HAL_SPI_DeInit(&hspi2) != HAL_OK) {
            Error_Handler();
        }
    }
}
}
}