#pragma once

#include <span>
#include <bitset>
#include "stm32h7xx_hal.h"

namespace i2c2 {
    namespace sw {
        class Bus {
        public:
            Bus();
            ~Bus();

            void set_SCL();
            void reset_SCL();
            void test_SCL();

            void set_SDA();
            void reset_SDA();
            void test_SDA();

            void delay();

            void start_cond(); 
            void stop_cond(); 
            void write_bit(const bool b);
            bool read_SDA();
            bool read_bit();

            bool write_byte(const std::bitset<8>& byte, const bool start, const bool stop);
            bool read_byte(const std::bitset<8>& byte, const bool start, const bool stop);
            std::bitset<8> read_byte(const bool ack, const bool stop);
            bool send_byte(uint8_t address, uint8_t data);
            uint8_t recv_byte(uint8_t address);

            // TODO
            void scan();
            HAL_StatusTypeDef is_device_ready(const uint16_t address);
            HAL_StatusTypeDef transmit(const uint16_t address, const std::span<uint8_t>& data);
            HAL_StatusTypeDef receive(const uint16_t address, const std::span<uint8_t>& data);
        };
    }
}