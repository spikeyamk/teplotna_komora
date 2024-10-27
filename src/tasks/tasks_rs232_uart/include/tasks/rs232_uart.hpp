#pragma once

#include <iostream>

#include "tasks/prototype.hpp"

namespace tasks {
    class RS232_UART : public Prototype<RS232_UART, configMINIMAL_STACK_SIZE, "rs232_uart"> {
    private:
        float rpm { 0.0f };
        RS232_UART() = default;
    public:
        static RS232_UART& get_instance();
        static void worker(void* arg);
    };
}