#pragma once

#include <span>
#include "stm32h7xx_hal.h"

namespace i2c2 {
    namespace sw {
        class Bus {
        public:
            Bus();
            ~Bus();
            static void init_clk();
            void set_SDA();
            void reset_SDA();
            void set_SCL();
            void reset_SCL();
            void scan();
            HAL_StatusTypeDef is_device_ready(const uint16_t address);
            HAL_StatusTypeDef transmit(const uint16_t address, const std::span<uint8_t>& data);
            HAL_StatusTypeDef receive(const uint16_t address, const std::span<uint8_t>& data);
        };
    }
}