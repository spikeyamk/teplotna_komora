#pragma once

#include "util/tmp.hpp"
#include "magic/commands/deserializer.hpp"

namespace comm {
namespace rs232_uart {
    template<typename T>
    class SemaphoreBase {
    public:
        using Derived = T;
        using CRTP = SemaphoreBase<Derived>;
    public:
        void release() {
            static_cast<Derived&>(*this).release();
        }

        void acquire() {
            static_cast<Derived&>(*this).acquire();
        }

        bool try_acquire_for(const uint32_t timeout_ms) {
            return static_cast<Derived&>(*this).try_acquire_for(timeout_ms);
        }
    };

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
            if((sem.try_acquire_for(timeout_ms) == false) || empty()) {
                std::cout << "pop_for: channel is empty returning nullopt...\n";
                return std::nullopt;
            }
            const std::optional<EventVariant> ret = event_variant;
            event_variant = std::nullopt;
            std::cout << "pop_for: emptying channel...\n";
            return ret;
        }
    };
}
}