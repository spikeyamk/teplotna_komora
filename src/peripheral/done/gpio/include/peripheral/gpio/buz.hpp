#pragma once

#include "stm32f2xx_hal.h"

namespace peripheral {
namespace gpio {
namespace buz {
    extern const GPIO_TypeDef* PORT;
    extern const uint16_t PIN;
    void init();
    void deinit();
}    
}
}