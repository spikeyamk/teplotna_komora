#pragma once

#include "util/tmp.hpp"
#include "magic/commands/deserializer.hpp"
#include "comm/rs232_uart/semaphore_base.hpp"

namespace comm {
namespace rs232_uart {
    template<typename T>
    requires std::is_base_of_v<SemaphoreBase<typename T::Derived>, T>
    class Channel {
    public:
        using Semaphore = T;
        using EventVariant = magic::commands::Deserializer::Variant;
    private:
        Semaphore sem {};
        std::optional<EventVariant> event_variant { std::nullopt };
    public:
        Channel() = default;

        void push(const Channel::EventVariant& obj) {
            event_variant = obj;
            sem.release();
        }

        bool empty() const {
            return event_variant.has_value() == false;
        }

        void wait_until_not_empty() {
            while(empty()) {
                sem.acquire();
            }
        }

        std::optional<EventVariant> pop_for(const uint32_t timeout_ms) {
            if(empty() && (sem.try_acquire_for(timeout_ms) == false)) {
                return std::nullopt;
            }
            const std::optional<EventVariant> ret = event_variant;
            event_variant = std::nullopt;
            return ret;
        }
    };
}
}