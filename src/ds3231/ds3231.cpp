#include <iostream>
#include "ds3231/ds3231.hpp"

namespace ds3231 {
    const uint32_t TIMEOUT { 100 };

    void scan_i2c_bus(I2C_HandleTypeDef* hi2cx) {
        std::printf("Scanning I2C bus...\n\r");
        // I2C addresses are 7 bits, so they range from 0x08 to 0x77
        for(uint8_t address = 0x08; address < 0xFF; address++) {
            // Attempt to communicate with the device
            if(HAL_I2C_IsDeviceReady(hi2cx, (address << 1), 3, TIMEOUT) == HAL_OK) {
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

    // Function to convert Binary Coded Decimal (BCD) to Decimal
    uint8_t bcd_to_decimal(uint8_t val) {
        return ((val / 16) * 10) + (val % 16);
    }

    // Function to read time registers from DS3231
    std::optional<std::array<uint8_t, 7>> ds3231_read_time(I2C_HandleTypeDef* hi2cx) {
        // DS3231 time starts at register 0x00 (seconds)
        uint8_t reg = 0x00;

        // Send the register address we want to read (starting from 0x00)
        if (HAL_I2C_Master_Transmit(hi2cx, DS3231_ADDRESS, &reg, 1, TIMEOUT) != HAL_OK) {
            std::printf("Error sending register address to DS3231\n\r");
            return std::nullopt;
        }

        std::array<uint8_t, 7> ret {};
        // Read 7 bytes of time data (seconds, minutes, hours, day, date, month, year)
        if (HAL_I2C_Master_Receive(hi2cx, DS3231_ADDRESS, ret.data(), ret.size(), TIMEOUT) != HAL_OK) {
            std::printf("Error reading time from DS3231\n\r");
            return std::nullopt;
        }

        return { ret };
    }

    // Function to display the time in human-readable format
    void print_time(const std::array<uint8_t, 7>& time_data) {
        uint8_t seconds = bcd_to_decimal(time_data[0]);
        uint8_t minutes = bcd_to_decimal(time_data[1]);
        uint8_t hours = bcd_to_decimal(time_data[2]);
        uint8_t day = bcd_to_decimal(time_data[3]);
        uint8_t date = bcd_to_decimal(time_data[4]);
        uint8_t month = bcd_to_decimal(time_data[5] & 0x1F); // Mask the century bit
        uint8_t year = bcd_to_decimal(time_data[6]);

        std::printf("Time: %02u:%02u:%02u, Date: %02u/%02u/20%02u\n\r", hours, minutes, seconds, date, month, year);
    }
}