#pragma once

#include <iostream>
#include <bitset>
#include <array>
#include "main.h"
#include "stm32f2xx_hal.h"

namespace panel {
namespace encoder {
    /*void encoder_test() {
        int counter = 0;
        
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET) {
            if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_RESET) {

                while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_RESET) {};
                std::printf("Counter: %d\n", counter++);
                while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET) {};
                HAL_Delay(10);
            }

            if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_SET) {

                while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_SET) {};
                std::printf("Counter: %d\n", counter--);
                while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10) == GPIO_PIN_RESET) {};
                while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_RESET) {};
                HAL_Delay(10);
            }

            if (counter < 0) counter = 0;
            if (counter > 16) counter = 16;
    }
    }*/

    enum class Direction {
        AntiClockWise,
        ClockWise,
    };

    using State = std::bitset<2>;

    struct Transition {
        State before;
        State after;
        Direction direction;
    };

    constexpr std::array<Transition, 8> transitions {{
        {
            .before = { 00 },
            .after = { 01 },
            .direction = Direction::AntiClockWise
        },
        {
            .before = { 00 },
            .after = { 10 },
            .direction = Direction::ClockWise
        },
        {
            .before = { 01 },
            .after = { 00 },
            .direction = Direction::ClockWise
        },
        {
            .before = { 01 },
            .after = { 11 },
            .direction = Direction::AntiClockWise
        },
        {
            .before = { 10 },
            .after = { 00 },
            .direction = Direction::AntiClockWise
        },
        {
            .before = { 10 },
            .after = { 11 },
            .direction = Direction::ClockWise
        },
        {
            .before = { 11 },
            .after = { 01 },
            .direction = Direction::ClockWise
        },
        {
            .before = { 11 },
            .after = { 10 },
            .direction = Direction::AntiClockWise
        },
    }};

    extern State previous_state;

    inline void test() {
        
        std::printf(
            "HAL_GPIO_ReadPin(ENCA_EXTI10_GPIO_Port, ENCA_EXTI10_Pin): 0x%02X\n",
            HAL_GPIO_ReadPin(ENCA_EXTI10_GPIO_Port, ENCA_EXTI10_Pin)
        );
        std::printf(
            "HAL_GPIO_ReadPin(ENCB_EXTI11_GPIO_Port, ENCB_EXTI11_Pin): 0x%02X\n",
            HAL_GPIO_ReadPin(ENCB_EXTI11_GPIO_Port, ENCB_EXTI11_Pin)
        );
    }
}
}
