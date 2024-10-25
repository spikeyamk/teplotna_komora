#pragma once

#include <cstdint>
#include "bksram/bksram.hpp"

namespace util {
    template<typename T>
    inline T& self_ref(void* arg) {
        T& ret { *reinterpret_cast<T*>(arg) };
        return ret;
    }

    template<typename T>
    inline const T& self_cref(void* arg) {
        const T& ret { *reinterpret_cast<const T*>(arg) };
        return ret;
    }

    inline void reset(const uint32_t magic) {
        bksram::write(magic);
        NVIC_SystemReset();
    }

    void turn_every_annoying_peripheral_off();

    inline void microsec_blocking_delay(const uint32_t us) {
        const uint32_t SYSTICK_LOAD { SystemCoreClock / 1000000U };
        const uint32_t SYSTICK_DELAY_CALIB { SYSTICK_LOAD >> 1 };

        do {
            const uint32_t start = SysTick->VAL;
            const uint32_t ticks = (us * SYSTICK_LOAD)-SYSTICK_DELAY_CALIB;
            while((start - SysTick->VAL) < ticks);
        } while (0);
    }
}