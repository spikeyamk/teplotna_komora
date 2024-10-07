#pragma once

#include <span>
#include "stm32h7xx_hal.h"

namespace i2c2 {
    namespace hw {
        class Bus {
            I2C_HandleTypeDef hi2c2 {};
        public:
            Bus();
            ~Bus();
            void scan();
            HAL_StatusTypeDef transmit(const uint16_t address, const std::span<uint8_t>& data);
            HAL_StatusTypeDef receive(const uint16_t address, const std::span<uint8_t>& data);
        };
    }
}