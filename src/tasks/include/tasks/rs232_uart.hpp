#pragma once

#include "usart.h"
#include "magic/magic.hpp"
#include "tasks/task.hpp"

namespace tasks {
    class RS232_UART : public TaskBase<RS232_UART, 4 * 1024, "rs232_uart"> {
        friend CRTP;
    private:
        StaticSemaphore_t semaphore_control_block {};
        osSemaphoreId_t semaphore { nullptr };

        static constexpr uint32_t semaphore_timeout { magic::TIMEOUT_MS };

        struct Connection {
            struct Actions {
                static void connect(RS232_UART& self);
                static void disconnect(RS232_UART& self);
                static void nop(RS232_UART& self);
                static void read_temp_ctl(const RS232_UART& self);
                static void write_temp(RS232_UART& self, const magic::commands::WriteTemp& write_temp);
            };

            auto operator()() const;
        };
    public:
        uint16_t rx_len { 0 };
    private:
        RS232_UART() = default;
    public:
        static RS232_UART& get_instance();
        osStatus release_semaphore(const uint16_t in_rx_len);
    private:
        void init();
        static void worker(void* arg);

        template<typename T>
        void transmit(const T& obj) const {
            const auto buf { magic::results::Serializer::run(obj) };
            static_assert(buf.size() <= magic::MTU);
            HAL_UART_Transmit(&huart3, buf.data(), buf.size(), HAL_MAX_DELAY);
        }
    };
}