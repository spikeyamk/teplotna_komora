#include <iostream>
#include <string_view>
#include <limits>
#include <cassert>

#include "usart.h"
#include "comm/redirect_stdout.hpp"

namespace comm {
    RedirectStdout& RedirectStdout::get_instance() {
        static RedirectStdout instance {};
        return instance;
    }

    osStatus_t RedirectStdout::acquire_mutex() {
        return osMutexAcquire(mutex, MUTEX_TIMEOUT_MS);
    }

    osStatus_t RedirectStdout::release_mutex() {
        return osMutexRelease(mutex);
    }

    RedirectStdout::RedirectStdout() {
        const osMutexAttr_t mutex_attr {
            .name = "usb_buf_mutex",
            .attr_bits = 0,
            .cb_mem = &mutex_control_block,
            .cb_size = sizeof(mutex_control_block),
        };
        
        mutex = osMutexNew(&mutex_attr);
        assert(mutex != nullptr);
    }

    bool RedirectStdout::push(const int ch) {
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
}