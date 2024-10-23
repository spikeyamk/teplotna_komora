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
        const auto ret { osMutexAcquire(&mutex, 1'000) };
        if(ret == osOK) {
            mutex_acquires++;
        }
        return ret;
    }

    osStatus_t RedirectStdout::release_mutex() {
        for(size_t i = 0; i < mutex_acquires; i++) {
            const auto ret { osMutexRelease(&mutex) };
            if(ret != osOK) {
                return ret;
            }
        }

        mutex_acquires = 0;
        return osOK;
    }

    bool RedirectStdout::init() {
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
    }

    bool RedirectStdout::get_threadsafe() const {
        const bool ret { threadsafe };
    }

    void RedirectStdout::turn_off_threadsafe() {
        threadsafe = false;
    }
}
}