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

    inline int __io_putchar(int ch) {
        if(xPortIsInsideInterrupt()) {
            RedirectStdout::transmit(ch);
            return ch;
        }

        taskENTER_CRITICAL();
        RedirectStdout& redirect_stdout { RedirectStdout::get_instance() };
        const bool threadsafe { redirect_stdout.get_threadsafe() };
        taskEXIT_CRITICAL();

        if(threadsafe == false) {
            RedirectStdout::transmit(ch);
            return ch;
        }

        const osStatus acquire_mutex { redirect_stdout.acquire_mutex() };
        if(acquire_mutex != osOK) {
            redirect_stdout.turn_off_threadsafe();
            RedirectStdout::transmit(ch);
            static constexpr std::string_view error_message { "\r\n__io_putchar: acquire_mutex != osOK\r\n" };
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>(error_message.data()), error_message.size(), HAL_MAX_DELAY);
            Error_Handler();
            return ch;
        }

        if(redirect_stdout.push(ch) == false || ch == '\n') {
            redirect_stdout.flush();
            redirect_stdout.release_mutex();
        }
        
        return ch;
    }
}
}