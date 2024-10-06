#include <trielo/trielo.hpp>
#include "i2c2/common.hpp"
#include "i2c2/sw.hpp"

namespace i2c2 {
    namespace sw {
        Bus::Bus() {
            Trielo::trielo<init_clk>();

            Trielo::trielo<HAL_GPIO_WritePin>(
                const_cast<GPIO_TypeDef*>(PORT),
                static_cast<uint16_t>(Pins::SDA) | static_cast<uint16_t>(Pins::SCL),
                GPIO_PIN_RESET
            );

            const GPIO_InitTypeDef GPIO_InitStruct {
                .Pin = static_cast<uint16_t>(Pins::SDA) | static_cast<uint16_t>(Pins::SCL),
                .Mode = GPIO_MODE_OUTPUT_OD,
                .Pull = GPIO_NOPULL ,
                .Speed = GPIO_SPEED_FREQ_LOW,
            };
            Trielo::trielo<HAL_GPIO_Init>(const_cast<GPIO_TypeDef*>(PORT), const_cast<GPIO_InitTypeDef*>(&GPIO_InitStruct));
            set_SDA();
            set_SCL();
        }

        void Bus::init_clk() {
            __HAL_RCC_GPIOF_CLK_ENABLE();
        }

        void Bus::set_SDA() {
            HAL_GPIO_WritePin(const_cast<GPIO_TypeDef*>(PORT), static_cast<uint16_t>(Pins::SDA), GPIO_PIN_SET);
        }

        void Bus::reset_SDA() {
            HAL_GPIO_WritePin(const_cast<GPIO_TypeDef*>(PORT), static_cast<uint16_t>(Pins::SDA), GPIO_PIN_RESET);
        }

        void Bus::set_SCL() {
            HAL_GPIO_WritePin(const_cast<GPIO_TypeDef*>(PORT), static_cast<uint16_t>(Pins::SCL), GPIO_PIN_SET);
        }

        void Bus::reset_SCL() {
            HAL_GPIO_WritePin(const_cast<GPIO_TypeDef*>(PORT), static_cast<uint16_t>(Pins::SCL), GPIO_PIN_RESET);
        }

        Bus::~Bus() {

        }

        HAL_StatusTypeDef Bus::is_device_ready(const uint16_t address) {
            return HAL_ERROR;
        }

        void Bus::scan() {
            std::printf("Scanning I2C bus...\n\r");
            // I2C addresses are 7 bits, so they range from 0x08 to 0x77
            for(uint8_t address = 0x08; address < 0xFF; address++) {
                // Attempt to communicate with the device
                if(is_device_ready(address << 1) == HAL_OK
                    //HAL_I2C_IsDeviceReady(&hi2c2, (address << 1), 1, TIMEOUT) == HAL_OK
                ) {
                    // If the device responds, print its address
                    std::printf("Device found at 0x%02X\n\r", address);
                    const uint8_t ds3231_address { 0x68 };
                    if(address == ds3231_address) {
                        std::printf("address == ds3231_address\n\r");
                    }
                }
            }
            std::printf("I2C scan complete.\n\r");
        }

        HAL_StatusTypeDef Bus::transmit(const uint16_t address, const std::span<uint8_t>& data) {
            //return HAL_I2C_Master_Transmit(&hi2c2, address, data.data(), data.size(), TIMEOUT);
            return HAL_ERROR;
        }

        HAL_StatusTypeDef Bus::receive(const uint16_t address, const std::span<uint8_t>& data) {
            //return HAL_I2C_Master_Receive(&hi2c2, address, data.data(), data.size(), TIMEOUT);
            return HAL_ERROR;
        }
    }
}