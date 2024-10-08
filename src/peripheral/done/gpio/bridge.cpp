#include "peripheral/gpio/bridge.hpp"

namespace peripheral {
namespace gpio {
namespace bridge {
    namespace Pins {
        const PortPin BRIDGEA_C0 {
            .port = GPIOB,
            .pin = GPIO_PIN_0
        };
        const PortPin BRIDGEA_H0 {
            .port = GPIOB,
            .pin = GPIO_PIN_1
        };
        const PortPin BRIDGEA_H1 {
            .port = GPIOC,
            .pin = GPIO_PIN_4
        };
        const PortPin BRIDGEA_C1 {
            .port = GPIOC,
            .pin = GPIO_PIN_5
        };
        const PortPin BRIDGEB_C0 {
            .port = GPIOA,
            .pin = GPIO_PIN_8
        };
        const PortPin BRIDGEB_C1 {
            .port = GPIOC,
            .pin = GPIO_PIN_10
        };
        const PortPin BRIDGEB_H0 {
            .port = GPIOC,
            .pin = GPIO_PIN_11
        };
        const PortPin BRIDGEB_H1 {
            .port = GPIOC,
            .pin = GPIO_PIN_12
        };
    };

    void init() {
        {
            __HAL_RCC_GPIOA_CLK_ENABLE();

            HAL_GPIO_WritePin(
                const_cast<GPIO_TypeDef*>(Pins::BRIDGEB_C0.port),
                Pins::BRIDGEB_C0.pin,
                GPIO_PIN_RESET
            );

            const GPIO_InitTypeDef GPIO_InitStruct {
                .Pin = Pins::BRIDGEB_C0.pin,
                .Mode = GPIO_MODE_OUTPUT_PP,
                .Pull = GPIO_NOPULL,
                .Speed = GPIO_SPEED_FREQ_LOW,
                .Alternate = 0
            };

            HAL_GPIO_Init(
                const_cast<GPIO_TypeDef*>(Pins::BRIDGEB_C0.port),
                const_cast<GPIO_InitTypeDef*>(&GPIO_InitStruct)
            );
        }

        {
            __HAL_RCC_GPIOB_CLK_ENABLE();

            HAL_GPIO_WritePin(
                const_cast<GPIO_TypeDef*>(Pins::BRIDGEA_H0.port),
                Pins::BRIDGEA_H0.pin | Pins::BRIDGEA_C0.pin,
                GPIO_PIN_RESET
            );

            const GPIO_InitTypeDef GPIO_InitStruct {
                .Pin = Pins::BRIDGEA_H0.pin | Pins::BRIDGEA_C0.pin,
                .Mode = GPIO_MODE_OUTPUT_PP,
                .Pull = GPIO_NOPULL,
                .Speed = GPIO_SPEED_FREQ_LOW,
                .Alternate = 0
            };

            HAL_GPIO_Init(
                const_cast<GPIO_TypeDef*>(Pins::BRIDGEA_H0.port),
                const_cast<GPIO_InitTypeDef*>(&GPIO_InitStruct)
            );
        }
        
        {
            __HAL_RCC_GPIOC_CLK_ENABLE();

            HAL_GPIO_WritePin(
                const_cast<GPIO_TypeDef*>(Pins::BRIDGEA_H1.port),
                Pins::BRIDGEA_H1.pin
                | Pins::BRIDGEA_C1.pin
                | Pins::BRIDGEB_C1.pin
                | Pins::BRIDGEB_H0.pin
                | Pins::BRIDGEB_H1.pin,
                GPIO_PIN_RESET
            );

            const GPIO_InitTypeDef GPIO_InitStruct {
                .Pin =
                    Pins::BRIDGEA_H1.pin
                    | Pins::BRIDGEA_C1.pin
                    | Pins::BRIDGEB_C1.pin
                    | Pins::BRIDGEB_H0.pin
                    | Pins::BRIDGEB_H1.pin,
                .Mode = GPIO_MODE_OUTPUT_PP,
                .Pull = GPIO_NOPULL,
                .Speed = GPIO_SPEED_FREQ_LOW,
                .Alternate = 0
            };

            HAL_GPIO_Init(const_cast<GPIO_TypeDef*>(Pins::BRIDGEA_H1.port), const_cast<GPIO_InitTypeDef*>(&GPIO_InitStruct));
        }
    }

    void deinit() {
        HAL_GPIO_DeInit(
            const_cast<GPIO_TypeDef*>(Pins::BRIDGEB_C0.port),
            Pins::BRIDGEB_C0.pin
        );

        HAL_GPIO_DeInit(
            const_cast<GPIO_TypeDef*>(Pins::BRIDGEA_H0.port),
            Pins::BRIDGEA_H0.pin | Pins::BRIDGEA_C0.pin
        );

        HAL_GPIO_DeInit(
            const_cast<GPIO_TypeDef*>(Pins::BRIDGEA_H1.port),
            Pins::BRIDGEA_H1.pin
            | Pins::BRIDGEA_C1.pin
            | Pins::BRIDGEB_C1.pin
            | Pins::BRIDGEB_H0.pin
            | Pins::BRIDGEB_H1.pin
        );
    }
}    
}
}