#include "stm32h7xx_hal.h"
#include "clk/clk.hpp"

namespace clk {
    void init_i2c2() {
        __HAL_RCC_GPIOF_CLK_ENABLE();
    }
}