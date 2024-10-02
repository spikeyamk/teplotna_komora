#include <string_view>
#include <trielo/trielo.hpp>
#include "submodule/public.hpp"
#include "actu/fan/fan.hpp"
#include "returns_true.hpp"
#include "stm32f2xx_hal.h"
#include "main.h"
#include "app.hpp"

inline int redirect(int ch, UART_HandleTypeDef* huart1) {
    const uint32_t COM_POLL_TIMEOUT = 1000;
    HAL_UART_Transmit(huart1, (uint8_t *) &ch, 1, COM_POLL_TIMEOUT);
    //HAL_UART_Transmit_IT(huart1, (uint8_t *) &ch, 1);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
    HAL_Delay(100);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
    return ch;
}

/// This function calculates the area of a rectangle.
int app_main(int width, int height, UART_HandleTypeDef* huart1) {
    (void) width;
    (void) height;

    /* STM32H503x has 128K FLASH only these functions don't fit into it */
    Trielo::trielo<submodule::foo>();
    Trielo::trielo<returns_true>();

    actu::fan::stop_all();

    const std::string_view message { "Hello World!\n\r" };
    std::printf("%s", message.data());
    while(1) {
        //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
        for(const char c: message) {
            redirect(c, huart1);
        }
        HAL_Delay(500);
    }
    return 0;
}