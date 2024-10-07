#include <iostream>
#include <array>
#include <algorithm>
#include "cmsis_os2.h"
#include "i2c2/i2c2.hpp"
#include "i2c2/test.hpp"

namespace i2c2 {
    namespace hw {
        void test() {
            Bus().scan();
        }
    }

    namespace sw {
        void test() {
            Bus().scan();
        }
    }


            /*
    namespace stm32_bitbang_i2c {
        static uint8_t bcd_to_decimal(uint8_t val) {
            return ((val / 16) * 10) + (val % 16);
        }

        void test() {
            std::printf("i2c2::stm32_bitbang_i2c::test\n");
            init();
            scan();

            std::array<uint8_t, 7> time_data {};
            uint8_t reg { 0x00 };
            if(I2C_transmit((0x68 << 1), &reg, 1) == false) {
                std::printf("I2C_transmit((0x68 << 1), reg, ) == false\n");
            }

            if(I2C_receive((0x68 << 1), &reg, time_data.data(), 0, time_data.size()) == false) {
                std::printf("here5\n");
                std::printf("I2C_receive(0x68, reg, data.data(), 1, data.size()) == false\n");
                //return;
            }

            std::for_each(time_data.begin(), time_data.end(), [index = 0](const uint8_t e) mutable {
                std::printf("time_data[%u]: 0x%02X\n", index++, e);
            });

            const uint8_t seconds = bcd_to_decimal(time_data[0]);
            const uint8_t minutes = bcd_to_decimal(time_data[1]);
            const uint8_t hours = bcd_to_decimal(time_data[2]);
            const uint8_t day = bcd_to_decimal(time_data[3]);
            const uint8_t date = bcd_to_decimal(time_data[4]);
            const uint8_t month = bcd_to_decimal(time_data[5] & 0x1F); // Mask the century bit
            const uint8_t year = bcd_to_decimal(time_data[6]);

            std::printf("Time: %02u:%02u:%02u, Date: %02u/%02u/20%02u\n\r", hours, minutes, seconds, date, month, year);

            deinit();
        }
    }
            */
}