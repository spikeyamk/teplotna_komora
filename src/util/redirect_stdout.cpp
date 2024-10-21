#include "usart.h"
#include "cmsis_os2.h"

#include "util/reset.hpp"
#include "util/self_ref.hpp"
#include "util/redirect_stdout.hpp"
#include "util/is_irq.hpp"

namespace util {
    RedirectStdout& RedirectStdout::get_instance() {
        static RedirectStdout instance {};
        return instance;
    }

    bool RedirectStdout::init() {
        const osMessageQueueAttr_t queue_attr {
            .name = "stdout_queue",
            .attr_bits = 0,
            .cb_mem = &queue_control_block,
            .cb_size = sizeof(queue_control_block),
            .mq_mem = queue_data_storage.data(),
            .mq_size = queue_data_storage.size(),
        };

        queue = osMessageQueueNew(msg_count, msg_size, &queue_attr);
        if (queue == nullptr) {
            turn_off_threadsafe();
            return false;
        }

        const osThreadAttr_t consumer_attr {
            .name = "consumer",
            .attr_bits = osThreadDetached,
            .cb_mem = &task_control_block,
            .cb_size = sizeof(task_control_block),
            .stack_mem = &task_buffer[0],
            .stack_size = sizeof(task_buffer),
            .priority = (osPriority_t) osPriorityNormal,
            .tz_module = 0,
            .reserved = 0,
        };

        if (osThreadNew(worker, reinterpret_cast<void*>(this), &consumer_attr) == nullptr) {
            turn_off_threadsafe();
            return false;
        }

        return true;
    }
    
    void RedirectStdout::worker(void* arg) {
        RedirectStdout& self { *reinterpret_cast<RedirectStdout*>(arg) };
        std::array<uint32_t, msg_count * msg_size> local_data_storage {};
        size_t stopper = 0;

        for (;;) {
            self.threadsafe = true;
            int ch;
            if (osMessageQueueGet(self.queue, &ch, nullptr, osWaitForever) == osOK) {
                local_data_storage[stopper] = ch;
                stopper++;

                if (stopper == msg_count) {
                    transmit_storage(local_data_storage, stopper);
                    stopper = 0;
                }
            } else {
                self.turn_off_threadsafe();
            }
        }
    }

    void RedirectStdout::transmit_storage(const std::array<uint32_t, msg_count * msg_size>& storage, size_t stopper) {
        for (size_t i = 0; i < stopper; i++) {
            HAL_UART_Transmit(&huart1, reinterpret_cast<const uint8_t*>(&storage[i]), 1, HAL_MAX_DELAY);
        }
    }

    void RedirectStdout::new_line() {
        HAL_UART_Transmit(&huart1, reinterpret_cast<const uint8_t*>("\r"), 1, HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart1, reinterpret_cast<const uint8_t*>("\n"), 1, HAL_MAX_DELAY);
    }

    bool RedirectStdout::push(int ch) {
        if (threadsafe == false) {
            return false;
        }

        if (osMessageQueuePut(queue, &ch, 0, osWaitForever) != osOK) {
            turn_off_threadsafe();
            return false;
        }

        return true;
    }

    bool RedirectStdout::get_threadsafe() const {
        /*
        if(is_irq()) {
            const auto tmp { taskENTER_CRITICAL_FROM_ISR() };
            const bool ret { threadsafe };
            taskEXIT_CRITICAL_FROM_ISR(tmp);
            return ret;
        }

        taskENTER_CRITICAL();
        const bool ret { threadsafe };
        taskEXIT_CRITICAL();
        return ret;
        */
        return threadsafe;
    }

    void RedirectStdout::turn_off_threadsafe() {
        threadsafe = false;
    }

    void RedirectStdout::lock() {

    }
}