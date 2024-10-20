#include "stm32f2xx_hal.h"
#include "usart.h"
#include "util/redirect_stdout.hpp"

extern "C" int __io_putchar(int ch) {
    auto redirect_stdout { util::RedirectStdout::get_instance() };
    if(ch == '\n') {
        if(redirect_stdout.get_threadsafe()) {
            if(util::RedirectStdout::get_instance().push('\r') == false) {
                HAL_UART_Transmit(&huart1, reinterpret_cast<const uint8_t*>("\r"), 1, 1000);
            } else if(util::RedirectStdout::get_instance().push('\n') == false) {
                HAL_UART_Transmit(&huart1, reinterpret_cast<const uint8_t*>("\n"), 1, 1000);
            }
        } else {
            util::RedirectStdout::new_line();
            HAL_UART_Transmit(&huart3, reinterpret_cast<const uint8_t*>("\r\n"), 2, 1000);
        }
    } else {
        if(redirect_stdout.get_threadsafe()) {
            if(util::RedirectStdout::get_instance().push(ch) == false) {
                HAL_UART_Transmit(&huart1, reinterpret_cast<uint8_t *>(&ch), 1, 1000);
            }
        } else {
            HAL_UART_Transmit(&huart1, reinterpret_cast<uint8_t *>(&ch), 1, 1000);
            HAL_UART_Transmit(&huart3, reinterpret_cast<uint8_t *>(&ch), 1, 1000);
        }
    }
    return ch;
}