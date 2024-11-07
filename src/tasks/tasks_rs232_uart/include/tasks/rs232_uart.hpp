#pragma once

#include "usart.h"
#include "common/magic/magic.hpp"
#include "tasks/prototype.hpp"

namespace tasks {
    class RS232_UART : public Prototype<RS232_UART, 4 * 1024, "rs232_uart"> {
        friend CRTP;
    private:
        StaticSemaphore_t semaphore_control_block {};
        osSemaphoreId_t semaphore { nullptr };

        static constexpr uint32_t semaphore_timeout { common::magic::TIMEOUT_MS };

        struct Connection {
            struct Actions {
                static void connect(RS232_UART& self);
                static void disconnect(RS232_UART& self);
                static void nop(RS232_UART& self);
                static void read_sensors(const RS232_UART& self);
                static void write_temp(RS232_UART& self, const common::magic::commands::WriteTemp& write_temp);
            };

            auto operator()() const;
        };
    public:
        uint16_t rx_len { 0 };
    private:
        RS232_UART() = default;
    public:
        static RS232_UART& get_instance();
        bool init();
        osStatus release_semaphore(const uint16_t in_rx_len);
    private:
        static void worker(void* arg);

        template<typename T>
        void transmit(const T& obj) const {
            const auto buf { common::magic::results::Serializer::run(obj) };
            static_assert(buf.size() <= common::magic::MTU);
            HAL_UART_Transmit(&huart3, buf.data(), buf.size(), HAL_MAX_DELAY);
        }
    };
}