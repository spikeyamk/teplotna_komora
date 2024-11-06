#include "tasks/rs232_uart.hpp"

namespace tasks {
    RS232_UART& RS232_UART::get_instance() {
        static RS232_UART instance {};
        return instance;
    }

    void RS232_UART::worker(void* arg) {
        RS232_UART& self { *static_cast<RS232_UART*>(arg) };
        (void) self;
        while(1) {
            osDelay(1'000);
        }
    }
}