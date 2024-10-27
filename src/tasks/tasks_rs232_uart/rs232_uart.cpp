#include "tasks/rs232_uart.hpp"

namespace tasks {
    RS232_UART& RS232_UART::get_instance() {
        static RS232_UART instance {};
        return instance;
    }

    void RS232_UART::worker(void* arg) {
        RS232_UART& self { *static_cast<RS232_UART*>(arg) };
        while(1) {
            std::printf("tasks::RS232_UART::worker: self.rpm: %f\n", self.rpm);
            osDelay(1'000);
        }
    }
}