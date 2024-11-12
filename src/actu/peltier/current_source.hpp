#pragma once

#include <bitint.hpp>
#include "stm32f2xx_hal.h"

namespace actu {
namespace peltier {
namespace current_source {
    void test();
    using uint12_t = bitint::ubitint<12>;
namespace front {
    void init();
    void set(const uint12_t code);
    uint12_t get_code();
}
namespace rear {
    void init();
    void set(const uint12_t code);
    uint12_t get_code();
}
}
}
}