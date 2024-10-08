#include "peripheral/gpio/i2cbb.hpp"

namespace peripheral {
namespace gpio {
namespace i2cbb {
    const GPIO_TypeDef* PORT { GPIOB };

    void init() {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        HAL_GPIO_WritePin(
            const_cast<GPIO_TypeDef*>(PORT),
            static_cast<uint16_t>(Pins::I2CBB1_SCL)
            | static_cast<uint16_t>(Pins::I2CBB1_SDA)
            , GPIO_PIN_RESET
        );

        const GPIO_InitTypeDef GPIO_InitStruct {
            .Pin = static_cast<uint16_t>(Pins::I2CBB1_SCL)
                | static_cast<uint16_t>(Pins::I2CBB1_SDA),
            .Mode = GPIO_MODE_OUTPUT_OD,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW,
            .Alternate = 0
        };
        HAL_GPIO_Init(const_cast<GPIO_TypeDef*>(PORT), const_cast<GPIO_InitTypeDef*>(&GPIO_InitStruct));
    }

    void deinit() {
        HAL_GPIO_DeInit(
            const_cast<GPIO_TypeDef*>(PORT),
            static_cast<uint16_t>(Pins::I2CBB1_SCL)
            | static_cast<uint16_t>(Pins::I2CBB1_SDA)
        );
    }
}
}
}