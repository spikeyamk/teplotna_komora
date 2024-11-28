#include "util/sml.hpp"
#include "comm/rs232_uart/connection.hpp"

class Semaphore : comm::rs232_uart::SemaphoreBase<Semaphore> {
public:
    void release() {

    }

    void acquire() {

    }

    bool try_acquire_for(const uint32_t timeout_ms) {

    }
};

int main() {
    util::sml::dump_uml<comm::rs232_uart::Connection<Semaphore>>(std::cout);
}