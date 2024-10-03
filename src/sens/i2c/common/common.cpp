#include <iostream>
#include "sens/i2c/common/common.hpp"

namespace sens {
namespace i2c {
namespace common {
    void scan(I2C_HandleTypeDef* hi2c1) {
        std::printf("Scanning I2C bus...\n\r");
        // I2C addresses are 7 bits, so they range from 0x08 to 0x77
        for(uint8_t address = 0x08; address <= 0x77; address++) {
            // Attempt to communicate with the device
            if(HAL_I2C_IsDeviceReady(hi2c1, (address << 1), 1, HAL_MAX_DELAY) == HAL_OK) {
                // If the device responds, print its address
                std::printf("Device found at 0x%02X\n\r", address);
            }
        }
        std::printf("I2C scan complete.\n\r");
    }
}
}
}