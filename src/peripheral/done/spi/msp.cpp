#include "peripheral/spi/spi1_temp.hpp"
#include "peripheral/spi/spi2_sevyg.hpp"

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(spiHandle->Instance==SPI1) {
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        using namespace peripheral::spi::spi1_temp;
        HAL_GPIO_WritePin(
            const_cast<GPIO_TypeDef*>(Ports::MOSI_MISO_SCK), 
            static_cast<uint16_t>(Pins::SPI1_TEMP_SCK)
            | static_cast<uint16_t>(Pins::SPI1_TEMP_MISO)
            | static_cast<uint16_t>(Pins::SPI1_TEMP_MOSI),
            GPIO_PIN_RESET
        );
        GPIO_InitStruct.Pin = static_cast<uint16_t>(Pins::SPI1_TEMP_SCK)
            | static_cast<uint16_t>(Pins::SPI1_TEMP_MISO)
            | static_cast<uint16_t>(Pins::SPI1_TEMP_MOSI);
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(const_cast<GPIO_TypeDef*>(Ports::MOSI_MISO_SCK), &GPIO_InitStruct);

        HAL_GPIO_WritePin(
            const_cast<GPIO_TypeDef*>(Ports::SS0_SS1), 
            static_cast<uint16_t>(Pins::SPI1_TEMP_SS0)
            | static_cast<uint16_t>(Pins::SPI1_TEMP_SS1),
            GPIO_PIN_RESET
        );
        GPIO_InitStruct.Pin = static_cast<uint16_t>(Pins::SPI1_TEMP_SS0)
            | static_cast<uint16_t>(Pins::SPI1_TEMP_SS1);
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = 0;
        HAL_GPIO_Init(const_cast<GPIO_TypeDef*>(Ports::SS0_SS1), &GPIO_InitStruct);
    } else if(spiHandle->Instance==SPI2) {
        __HAL_RCC_SPI2_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        using namespace peripheral::spi::spi2_sevyg;
        GPIO_InitStruct.Pin =
            static_cast<uint16_t>(Pins::SPI2_SEVYG_NSS)
            | static_cast<uint16_t>(Pins::SPI2_SEVYG_SCK)
            | static_cast<uint16_t>(Pins::SPI2_SEVYG_MISO)
            | static_cast<uint16_t>(Pins::SPI2_SEVYG_MOSI);
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(const_cast<GPIO_TypeDef*>(PORT), &GPIO_InitStruct);
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle) {
    if(spiHandle->Instance==SPI1) {
        __HAL_RCC_SPI1_CLK_DISABLE();
        using namespace peripheral::spi::spi1_temp;
        HAL_GPIO_DeInit(
            const_cast<GPIO_TypeDef*>(Ports::MOSI_MISO_SCK),
            static_cast<uint16_t>(Pins::SPI1_TEMP_SCK)
            | static_cast<uint16_t>(Pins::SPI1_TEMP_MISO)
            | static_cast<uint16_t>(Pins::SPI1_TEMP_MOSI)
        );
        HAL_GPIO_WritePin(
            const_cast<GPIO_TypeDef*>(Ports::SS0_SS1), 
            static_cast<uint16_t>(Pins::SPI1_TEMP_SS0)
            | static_cast<uint16_t>(Pins::SPI1_TEMP_SS1),
            GPIO_PIN_RESET
        );
    } else if(spiHandle->Instance==SPI2) {
        __HAL_RCC_SPI2_CLK_DISABLE();
        using namespace peripheral::spi::spi2_sevyg;
        HAL_GPIO_DeInit(
            const_cast<GPIO_TypeDef*>(PORT),
            static_cast<uint16_t>(Pins::SPI2_SEVYG_NSS)
            | static_cast<uint16_t>(Pins::SPI2_SEVYG_SCK)
            | static_cast<uint16_t>(Pins::SPI2_SEVYG_MISO)
            | static_cast<uint16_t>(Pins::SPI2_SEVYG_MOSI)
        );
    }
}