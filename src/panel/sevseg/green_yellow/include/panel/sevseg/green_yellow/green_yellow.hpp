#pragma once

#include "stm32f2xx_hal.h"

namespace panel {
namespace sevseg {
namespace green_yellow {
    void write(uint8_t address, uint8_t data);
    void init();
    void test();
}
}
}