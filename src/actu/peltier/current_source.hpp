#pragma once

#include <ubitint.hpp>
#include "stm32f2xx_hal.h"

namespace actu {
namespace current_source {
    void test_dac();
    using uint12_t = ubitint_t<12>;
namespace front {
    void start_dac();
    void stop_dac();
    void set_output(const uint12_t value);
    uint12_t read_output();
}
namespace rear {
    void start_dac();
    void stop_dac();
    void set_output(const uint12_t value);
    uint12_t read_output();
}
}
}