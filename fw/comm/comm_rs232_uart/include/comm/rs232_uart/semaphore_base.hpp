#pragma once

#include <cstdint>

#include "magic/results/deserializer.hpp"

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

}
}