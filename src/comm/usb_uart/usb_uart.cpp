#include <iostream>
#include <string_view>
#include <limits>

#include "usart.h"
#include "comm/usb_uart/usb_uart.hpp"

namespace comm {
namespace usb_uart {
    RedirectStdout& RedirectStdout::get_instance() {
        static RedirectStdout instance {};
        return instance;
    }

    size_t RedirectStdout::get_data_count() const {
        return data_count;
    }

    osStatus_t RedirectStdout::acquire_mutex() {
        const auto ret { osMutexAcquire(mutex, 10'000) };
        if(ret == osOK) {
            mutex_acquires++;
        }
        return ret;
    }

    osStatus_t RedirectStdout::release_mutex() {
        for(size_t i = 0; i < mutex_acquires; i++) {
            const auto ret { osMutexRelease(mutex) };
            if(ret != osOK) {
                return ret;
            }
        }

        mutex_acquires = 0;
        return osOK;
    }

    bool RedirectStdout::init_threadsafe() {
        const osMutexAttr_t mutex_attr {
            .name = "usb_buf_mutex",
            .attr_bits = osMutexRecursive,
            .cb_mem = &mutex_control_block,
            .cb_size = sizeof(mutex_control_block),
        };
        
        mutex = osMutexNew(&mutex_attr);
        if(mutex == nullptr) {
            return false;
        }

        threadsafe = true;
        return true;
    }

    bool RedirectStdout::push(int ch) {
        if(data_count == data_buffer.size()) {
            return false;
        }
        data_buffer[data_count] = ch;
        data_count++;
        return true;
    }

    void RedirectStdout::flush() {
        static_assert(
            std::tuple_size_v<std::remove_reference_t<decltype(data_buffer)>>
            * sizeof(std::remove_reference_t<decltype(data_buffer)>::value_type)
            < std::numeric_limits<uint16_t>::max()
        );
        HAL_UART_Transmit(
            &huart1,
            reinterpret_cast<const uint8_t*>(data_buffer.data()),
            data_count * sizeof(std::remove_reference_t<decltype(data_buffer)>::value_type),
            HAL_MAX_DELAY
        );
        data_count = 0;
    }

    bool RedirectStdout::get_threadsafe() const {
        const bool ret { threadsafe };
        return ret;
    }

    void RedirectStdout::turn_off_threadsafe() {
        threadsafe = false;
    }

    int __io_putchar(int ch) {
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
            HAL_UART_Transmit(&huart1, reinterpret_cast<const uint8_t*>(error_message.data()), error_message.size(), HAL_MAX_DELAY);
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