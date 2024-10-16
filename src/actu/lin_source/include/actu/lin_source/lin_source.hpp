#pragma once

#include "stm32f2xx_hal.h"

namespace actu {
namespace lin_source {
    void start_dac();
    void set_output(uint32_t val_a, uint32_t val_b);
}
}