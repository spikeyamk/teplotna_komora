#pragma once

#include <array>
#include <algorithm>
#include <optional>
#include "stm32f2xx_hal.h"

namespace ds3231 {
    template<typename T_i2cbb>
    class DS3231 {
        T_i2cbb& i2cbb;
        static constexpr uint16_t ADDRESS { 0x68 << 1 };
    public:
        DS3231(T_i2cbb& i2cbb) :
            i2cbb { i2cbb }
        {}

        static uint8_t bcd_to_decimal(uint8_t val) {
            return ((val / 16) * 10) + (val % 16);
        }

        std::optional<std::array<uint8_t, 7>> read_time() {
            // DS3231 time starts at register 0x00 (seconds)
            std::array<uint8_t, 1> reg { 0x00 };

            // Send the register address we want to read (starting from 0x00)
            if (i2cbb.transmit(ADDRESS, reg) != HAL_OK) {
                std::printf("Error sending register address to DS3231\n\r");
                return std::nullopt;
            }

            std::array<uint8_t, 7> ret {};
            // Read 7 bytes of time data (seconds, minutes, hours, day, date, month, year)
            if (i2cbb.receive(ADDRESS, ret) != HAL_OK) {
                std::printf("Error reading time from DS3231\n\r");
                return std::nullopt;
            }

            return { ret };
        }

        void print_time() {
            const auto time_data { read_time() };
            if(time_data.has_value() == false) {
                std::printf("print_time: time_data.has_value() == false\n\r");
                return;
            }
            
            std::for_each(time_data.value().begin(), time_data.value().end(), [index = 0](const uint8_t e) mutable {
                std::printf("time_data[%u]: 0x%02X\n", index++, e);
            });

            const uint8_t seconds = bcd_to_decimal(time_data.value()[0]);
            const uint8_t minutes = bcd_to_decimal(time_data.value()[1]);
            const uint8_t hours = bcd_to_decimal(time_data.value()[2]);
            const uint8_t day = bcd_to_decimal(time_data.value()[3]);
            const uint8_t date = bcd_to_decimal(time_data.value()[4]);
            const uint8_t month = bcd_to_decimal(time_data.value()[5] & 0x1F); // Mask the century bit
            const uint8_t year = bcd_to_decimal(time_data.value()[6]);

            std::printf("Time: %02u:%02u:%02u, Day: %02u, Date: %02u/%02u/20%02u\n\r", hours, minutes, seconds, day, date, month, year);
        }
    };
}