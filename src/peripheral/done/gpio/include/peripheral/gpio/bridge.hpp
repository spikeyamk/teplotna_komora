#pragma once

#include "stm32f2xx_hal.h"

namespace peripheral {
namespace gpio {
namespace bridge {
    struct PortPin {
        GPIO_TypeDef* port;
        uint32_t pin;
    };

    namespace Pins {
        extern const PortPin BRIDGEA_C0;
        extern const PortPin BRIDGEA_H0;
        extern const PortPin BRIDGEA_H1;
        extern const PortPin BRIDGEA_C1;
        extern const PortPin BRIDGEB_C0;
        extern const PortPin BRIDGEB_H0;
        extern const PortPin BRIDGEB_H1;
        extern const PortPin BRIDGEB_C1;
    };

    void init();
    void deinit();
}    
}
}