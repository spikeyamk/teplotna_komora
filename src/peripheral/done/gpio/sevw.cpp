#include "peripheral/gpio/sevw.hpp"

namespace peripheral {
namespace gpio {
namespace sevw {
    const GPIO_TypeDef* PORT { GPIOE };

    void init() {
        __HAL_RCC_GPIOE_CLK_ENABLE();
        HAL_GPIO_WritePin(
            const_cast<GPIO_TypeDef*>(PORT),
            static_cast<uint8_t>(Pins::SEVW_CC0_Pin)
            | static_cast<uint8_t>(Pins::SEVW_CC1_Pin)
            | static_cast<uint8_t>(Pins::SEVW_CC2_Pin)
            | static_cast<uint8_t>(Pins::SEVW_CC3_Pin)
            | static_cast<uint8_t>(Pins::SEVW_CC4_Pin)
            | static_cast<uint8_t>(Pins::SEVW_A_Pin)
            | static_cast<uint8_t>(Pins::SEVW_B_Pin)
            | static_cast<uint8_t>(Pins::SEVW_D_Pin)
            | static_cast<uint8_t>(Pins::SEVW_E_Pin)
            | static_cast<uint8_t>(Pins::SEVW_F_Pin)
            | static_cast<uint8_t>(Pins::SEVW_G_Pin)
            | static_cast<uint8_t>(Pins::SEVW_DP_Pin),
            GPIO_PIN_RESET
        );

        const GPIO_InitTypeDef GPIO_InitStruct {
            .Pin = (
                static_cast<uint8_t>(Pins::SEVW_CC0_Pin)
                | static_cast<uint8_t>(Pins::SEVW_CC1_Pin)
                | static_cast<uint8_t>(Pins::SEVW_CC2_Pin)
                | static_cast<uint8_t>(Pins::SEVW_CC3_Pin)
                | static_cast<uint8_t>(Pins::SEVW_CC4_Pin)
                | static_cast<uint8_t>(Pins::SEVW_A_Pin)
                | static_cast<uint8_t>(Pins::SEVW_B_Pin)
                | static_cast<uint8_t>(Pins::SEVW_D_Pin)
                | static_cast<uint8_t>(Pins::SEVW_E_Pin)
                | static_cast<uint8_t>(Pins::SEVW_F_Pin)
                | static_cast<uint8_t>(Pins::SEVW_G_Pin)
                | static_cast<uint8_t>(Pins::SEVW_DP_Pin)
            ),
            .Mode = GPIO_MODE_OUTPUT_PP,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW,
            .Alternate = 0
        };

        HAL_GPIO_Init(const_cast<GPIO_TypeDef*>(PORT), const_cast<GPIO_InitTypeDef*>(&GPIO_InitStruct));
    }

    void deinit() {
        HAL_GPIO_DeInit(
            const_cast<GPIO_TypeDef*>(PORT),
            static_cast<uint8_t>(Pins::SEVW_CC0_Pin)
            | static_cast<uint8_t>(Pins::SEVW_CC1_Pin)
            | static_cast<uint8_t>(Pins::SEVW_CC2_Pin)
            | static_cast<uint8_t>(Pins::SEVW_CC3_Pin)
            | static_cast<uint8_t>(Pins::SEVW_CC4_Pin)
            | static_cast<uint8_t>(Pins::SEVW_A_Pin)
            | static_cast<uint8_t>(Pins::SEVW_B_Pin)
            | static_cast<uint8_t>(Pins::SEVW_D_Pin)
            | static_cast<uint8_t>(Pins::SEVW_E_Pin)
            | static_cast<uint8_t>(Pins::SEVW_F_Pin)
            | static_cast<uint8_t>(Pins::SEVW_G_Pin)
            | static_cast<uint8_t>(Pins::SEVW_DP_Pin)
        );
    }
}    
}
}