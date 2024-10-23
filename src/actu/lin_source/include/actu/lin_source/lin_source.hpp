#pragma once

#include "stm32f2xx_hal.h"

namespace actu {
namespace lin_source {
namespace lin_source {
    void test_dac();
}
namespace front {
    void start_dac();
    void set_output(const uint32_t value);
}
namespace rear {
    void start_dac();
    void set_output(const uint32_t value);
}
}
}