#pragma once

#include "bksram/bksram.hpp"

namespace util {
    inline void reset(const uint32_t magic) {
        bksram::write(magic);
        NVIC_SystemReset();
    }
}