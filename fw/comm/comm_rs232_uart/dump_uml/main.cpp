#include "util/sml.hpp"
#include "comm/rs232_uart/connection.hpp"
#include "comm/rs232_uart/semaphore_base.hpp"
#include "comm/rs232_uart/transmitter_base.hpp"

class Semaphore : comm::rs232_uart::SemaphoreBase<Semaphore> {
public:
    void release() {}
    void acquire() {}
    bool try_acquire_for(const uint32_t timeout_ms) {}
};

class Transmitter : public comm::rs232_uart::TransmitterBase<Transmitter> {
    friend CRTP;
private:
    void operator()(const ResultVariant& result_variant) const;
};


int main() {
    util::sml::dump_uml<comm::rs232_uart::Connection<Semaphore, Transmitter>>(std::cout);
}