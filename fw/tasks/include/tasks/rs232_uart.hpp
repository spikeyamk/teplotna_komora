#pragma once

#include "comm/rs232_uart/rs232_uart.hpp"
#include "tasks/task.hpp"

namespace tasks {
    class RS232_UART : public TaskBase<RS232_UART, 4 * 1024, "rs232_uart"> {
        friend CRTP;
    public:
        using Receiver = comm::rs232_uart::Receiver<comm::rs232_uart::Semaphore>;
        Receiver receiver {};
    private:
        RS232_UART() = default;
    public:
        static RS232_UART& get_instance();
    private:
        static void worker(void* arg);
    };
}