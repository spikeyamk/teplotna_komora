#include <iostream>
#include <array>
#include "cmsis_os2.h"
#include "i2c2/i2c2.hpp"
#include "i2c2/test.hpp"
#include "stm32_bitbang_i2c/stm32_sw_i2c.h"

namespace i2c2 {
    namespace hw {
        void test() {
            Bus().scan();
        }
    }

    namespace sw {
        void test() {
            Bus bus;
            std::printf("bus.is_device_ready(0x68 << 1): %u\n",
                bus.is_device_ready(0x68 << 1)
            );
            //bus.scan();
        }
    }

    namespace stm32_bitbang_i2c {
        static uint8_t bcd_to_decimal(uint8_t val) {
            return ((val / 16) * 10) + (val % 16);
        }

        void test() {
            std::printf("stm32_bitbang_i2c::test\n");
            GPIO_InitTypeDef GPIO_InitStruct { 0 };
            /*Configure GPIO pins : SW_I2C_SCL_Pin SW_I2C_SDA_Pin */
            GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
            std::printf("here1\n");
            HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
            std::printf("here2\n");
            I2C_init();
            std::printf("here3\n");
            uint8_t reg[] { 0x00 };
            std::array<uint8_t, 7> time_data {};
            if(I2C_receive(0x68, reg, time_data.data(), 1, time_data.size()) == false) {
                std::printf("here5\n");
                std::printf("I2C_receive(0x68, reg, data.data(), 1, data.size()) == false\n");
                return;
            }
            std::printf("here6\n");

            const uint8_t seconds = bcd_to_decimal(time_data[0]);
            const uint8_t minutes = bcd_to_decimal(time_data[1]);
            const uint8_t hours = bcd_to_decimal(time_data[2]);
            const uint8_t day = bcd_to_decimal(time_data[3]);
            const uint8_t date = bcd_to_decimal(time_data[4]);
            const uint8_t month = bcd_to_decimal(time_data[5] & 0x1F); // Mask the century bit
            const uint8_t year = bcd_to_decimal(time_data[6]);

            std::printf("Time: %02u:%02u:%02u, Date: %02u/%02u/20%02u\n\r", hours, minutes, seconds, date, month, year);
        }
    }
}