#pragma once

#include <span>
#include "stm32f2xx_hal.h"
#include "core_cm3.h"

namespace i2cbb {
    class Bus {
    private:
        const GPIO_TypeDef* port;
        const uint16_t sda;
        const uint16_t scl;
    public:
        Bus(const GPIO_TypeDef* port, const uint16_t sda, const uint16_t scl);
        ~Bus();

        // https://deepbluembedded.com/stm32-delay-microsecond-millisecond-utility-dwt-delay-timer-delay/
        inline void delay(uint32_t us = 5) {
            const uint32_t SYSTICK_LOAD { SystemCoreClock / 1000000U };
            const uint32_t SYSTICK_DELAY_CALIB { SYSTICK_LOAD >> 1 };

            do {
                const uint32_t start = SysTick->VAL;
                const uint32_t ticks = (us * SYSTICK_LOAD)-SYSTICK_DELAY_CALIB;
                while((start - SysTick->VAL) < ticks);
            } while (0);
        }
    
        void set_SCL();
        void set_SDA();
        void reset_SCL();
        void reset_SDA();
    
        void start_cond();
        void stop_cond();
        void write_bit(const uint8_t b);
        uint8_t read_SDA();
        uint8_t read_bit();
        bool write_byte(uint8_t B, const bool start, const bool stop);
        uint8_t read_byte(const bool ack, const bool stop);
        bool send_byte(const uint8_t address, const uint8_t data);
        uint8_t receive_byte(const uint8_t address);
        bool send_byte_data(const uint8_t address, const uint8_t reg, const uint8_t data);
        uint8_t receive_byte_data(const uint8_t address, const uint8_t reg);
        HAL_StatusTypeDef transmit(const uint8_t address, const std::span<uint8_t>& data);
        HAL_StatusTypeDef receive(const uint8_t address, const std::span<uint8_t>& data);

        void scan();
    };
}