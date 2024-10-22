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
}