#pragma once

#include <array>

#include "stm32f2xx_hal.h"

namespace panel {
namespace led {
    class Led {
    private:
        GPIO_TypeDef* port;
        const uint16_t pin;
    public:
        Led(GPIO_TypeDef* port, const uint16_t pin) :
            port { port },
            pin { pin }
        {}

        void turn_on() const;
        void turn_off() const;
        void toggle() const;
    };

    extern const Led COOL;
    extern const Led HEAT;
    extern const Led RS232;
    extern const Led USB;

    extern const std::array<Led, 4> leds;
}
}