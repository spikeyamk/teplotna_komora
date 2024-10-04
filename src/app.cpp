#include <trielo/trielo.hpp>
#include "submodule/public.hpp"
#include "fanctl/bulk.hpp"
#include "stm32f2xx_hal.h"
#include "cmsis_os2.h"
#include "main.h"
#include "app.hpp"

volatile size_t produced_value { 0 };

void worker(void* arg) {
    UNUSED(arg);
    std::printf("worker: %u\n\r", produced_value++);
}

void consumer(void* arg) {
    UNUSED(arg);
    std::printf("worker: %u\n\r", produced_value--);
}

/// This function calculates the area of a rectangle.
void app_main() {
    /* STM32H503x has 128K FLASH only these functions don't fit into it */
    fanctl::stop_all();

    const osThreadAttr_t worker_attr {
        .name = "wokrer",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal,
    };
    osThreadNew(&worker, nullptr, &worker_attr);

    const osThreadAttr_t consumer_attr = {
        .name = "consumer",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal,
    }; 
    osThreadNew(&consumer, nullptr, &worker_attr);

    while(1) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
        std::printf("Hello World!\n\r");
        HAL_Delay(500);
    }
}