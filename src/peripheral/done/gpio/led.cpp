#include "peripheral/gpio/led.hpp"

namespace peripheral {
namespace gpio {
namespace led {
    const GPIO_TypeDef* PORT { GPIOC };

    void init() {
        __HAL_RCC_GPIOC_CLK_ENABLE();
        HAL_GPIO_WritePin(
            const_cast<GPIO_TypeDef*>(PORT),
            static_cast<uint16_t>(Pins::COOL_LED)
            | static_cast<uint16_t>(Pins::HEAT_LED)
            | static_cast<uint16_t>(Pins::RS232_LED)
            | static_cast<uint16_t>(Pins::USB_LED),
            GPIO_PIN_RESET
        );
        const GPIO_InitTypeDef GPIO_InitStruct {
            .Pin = (
                static_cast<uint16_t>(Pins::COOL_LED)
                | static_cast<uint16_t>(Pins::HEAT_LED)
                | static_cast<uint16_t>(Pins::RS232_LED)
                | static_cast<uint16_t>(Pins::USB_LED)
            ),
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW,
        };
        HAL_GPIO_Init(const_cast<GPIO_TypeDef*>(PORT), const_cast<GPIO_InitTypeDef*>(&GPIO_InitStruct));
    }

    void deinit() {
        HAL_GPIO_DeInit(
            const_cast<GPIO_TypeDef*>(PORT),
            static_cast<uint16_t>(Pins::COOL_LED)
            | static_cast<uint16_t>(Pins::HEAT_LED)
            | static_cast<uint16_t>(Pins::RS232_LED)
            | static_cast<uint16_t>(Pins::USB_LED)
        );
    }
}    
}
}