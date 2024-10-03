#pragma once

#include "stm32f2xx_hal.h"

namespace actu {
namespace lin_source {
    void start_dac(DAC_HandleTypeDef* hdac);
    void set_output(DAC_HandleTypeDef* hdac, uint32_t val_a, uint32_t val_b);
}
}