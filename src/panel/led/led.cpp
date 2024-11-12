#include "main.h"
#include "panel/led/led.hpp"

namespace panel {
namespace led {
    const Led COOL {
        LED0_COOL_GPIO_Port,
        LED0_COOL_Pin,
    };

    const Led HEAT {
        LED1_HEAT_GPIO_Port,
        LED1_HEAT_Pin,
    };

    const Led RS232 {
        LED2_RS232_GPIO_Port,
        LED2_RS232_Pin,
    };

    const Led USB {
        LED3_USB_GPIO_Port,
        LED3_USB_Pin,
    };

    extern const std::array<Led, 4> leds {{
        COOL,
        HEAT,
        RS232,
        USB
    }};

    void Led::turn_on() const {
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
    }

    void Led::turn_off() const {
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
    }

    void Led::toggle() const {
        HAL_GPIO_TogglePin(port, pin);
    }
}
}