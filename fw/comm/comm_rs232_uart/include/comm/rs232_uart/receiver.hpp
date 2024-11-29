#pragma once

#include "util/tmp.hpp"
#include "magic/magic.hpp"
#include "comm/rs232_uart/semaphore_base.hpp"

namespace comm {
namespace rs232_uart {
    template<typename T>
    requires std::is_base_of_v<SemaphoreBase<typename T::Derived>, T>
    class Receiver {
    public:
        using Semaphore = T;
        using EventVariant = magic::commands::Deserializer::Variant;
    private:
        Semaphore sem {};
        std::optional<uint16_t> rx_buf_len { std::nullopt };
    public:
        std::array<uint8_t, magic::MTU> rx_buf {};
    public:
        Receiver() = default;

        void push(const uint16_t new_rx_len) {
            if(empty() == false) {
                return;
            }
            rx_buf_len = new_rx_len;
            static_cast<Semaphore::CRTP&>(sem).release();
        }

        bool empty() const {
            return rx_buf_len.has_value() == false;
        }

        void wait_until_not_empty() {
            while(empty()) {
                static_cast<Semaphore::CRTP&>(sem).acquire();
            }
        }

        std::optional<EventVariant> pop_for(const std::chrono::milliseconds timeout) {
            if(empty() && (static_cast<Semaphore::CRTP&>(sem).try_acquire_for(timeout) == false)) {
                return std::nullopt;
            }

            if(rx_buf_len.value() > rx_buf.size()) {
                rx_buf_len = std::nullopt;
                return std::nullopt;
            }

            const std::optional<EventVariant> ret { magic::commands::Deserializer::decode(
                rx_buf.begin(),
                rx_buf.begin() + static_cast<size_t>(rx_buf_len.value())
            ) };

            rx_buf_len = std::nullopt;
            return ret;
        }
    };
}
}