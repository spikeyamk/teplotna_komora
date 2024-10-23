#include <string_view>
#include "comm/usb_uart/usb_uart.hpp"

extern "C" int __io_putchar(int ch) {
    using namespace comm::usb_uart;

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

    if(redirect_stdout.acquire_mutex() != osOK) {
        redirect_stdout.turn_off_threadsafe();
        RedirectStdout::transmit(ch);
        static constexpr std::string_view error_message { "\r\n__io_putchar: osMutexAcquire(&redirect_stdout.mutex, 1'000) != osOK\r\n" };
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