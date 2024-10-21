#pragma once

#include "cmsis_gcc.h"

namespace util {
    inline bool is_irq() {
        return (__get_IPSR() != 0U);
    }
}