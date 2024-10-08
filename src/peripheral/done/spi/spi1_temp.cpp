#include <iostream>
#include "peripheral/spi/spi1_temp.hpp"

namespace peripheral {
namespace spi {
namespace spi1_temp {
    namespace Ports {
        const GPIO_TypeDef* MOSI_MISO_SCK { GPIOD };
        const GPIO_TypeDef* SS0_SS1 { GPIOB };
    };

    void Error_Handler() {
        std::cout << "peripheral::spi::spi1_temp::Error_Handler\n";
    }

    void init() {
        hspi1.Instance = SPI1;
        hspi1.Init.Mode = SPI_MODE_MASTER;
        hspi1.Init.Direction = SPI_DIRECTION_2LINES;
        hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
        hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
        hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
        hspi1.Init.NSS = SPI_NSS_SOFT;
        hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
        hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
        hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
        hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        hspi1.Init.CRCPolynomial = 10;
        if(HAL_SPI_Init(&hspi1) != HAL_OK) {
            Error_Handler();
        }
    }

    void deinit() {
        if(HAL_SPI_DeInit(&hspi1) != HAL_OK) {
            Error_Handler();
        }
    }
}
}
}