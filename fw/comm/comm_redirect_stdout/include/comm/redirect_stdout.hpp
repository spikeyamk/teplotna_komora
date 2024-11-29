#pragma once

#include <array>

#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "usart.h"

namespace comm {
    class RedirectStdout {
    private:
        StaticSemaphore_t mutex_control_block;
        size_t data_count { 0 };
        std::array<int, 1024> data_buffer {};
        static const uint32_t MUTEX_TIMEOUT_MS { 10'000 };
    private:
        RedirectStdout();
    public:
        osMutexId_t mutex { nullptr };
    public:
        static HAL_StatusTypeDef inline transmit(const int ch) {
            if(ch == '\n') {
                return HAL_UART_Transmit(&huart1, reinterpret_cast<const uint8_t *>("\r\n"), 2, 1000);
            } else {
                return HAL_UART_Transmit(&huart1, reinterpret_cast<const uint8_t *>(&ch), 1, 1000);
            }
        }
        static RedirectStdout& get_instance();
        osStatus_t acquire_mutex();
        bool push(const int ch);
        void flush();
        osStatus_t release_mutex();
    };
}