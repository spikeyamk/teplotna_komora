#pragma once

#include <array>
#include <optional>
#include "stm32h7xx_hal.h"

extern I2C_HandleTypeDef hi2c2;

namespace ds3231 {
    extern const uint32_t TIMEOUT;
    constexpr uint8_t DS3231_ADDRESS { 0x68 << 1 };
    void scan_i2c_bus(I2C_HandleTypeDef* hi2cx);
    uint8_t bcd_to_decimal(uint8_t val);
    std::optional<std::array<uint8_t, 7>> ds3231_read_time(I2C_HandleTypeDef* hi2cx);
    void print_time(const std::array<uint8_t, 7>& time_data);
}