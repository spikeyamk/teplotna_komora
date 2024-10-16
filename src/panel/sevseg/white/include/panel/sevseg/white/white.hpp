#pragma once

#include "stm32f2xx_hal.h"

namespace panel {
namespace sevseg {
namespace white {
    void init_brightness();
    void dim();
    void bright();
    void turn_on_all_segments();
}
}
}