#pragma once

#include <array>
#include <string_view>

#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "usart.h"

namespace comm {
namespace usb_uart {
    class RedirectStdout {
    private:
        bool threadsafe { false };

        StaticSemaphore_t mutex_control_block;
        size_t mutex_acquires { 0 };

        size_t data_count { 0 };
        std::array<int, 1024> data_buffer {};
    private:
        RedirectStdout() = default;
    public:
        osMutexId_t mutex { nullptr };
        bool init();
    public:
        static void inline transmit(const int ch) {
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t *>(&ch), 1, 1000);
        }
        static RedirectStdout& get_instance();
        osStatus_t acquire_mutex();
        bool push(int ch);
        void flush();
        osStatus_t release_mutex();
        bool get_threadsafe() const;
        size_t get_data_count() const;
        void turn_off_threadsafe();
    };

    int __io_putchar(int ch);
}
}