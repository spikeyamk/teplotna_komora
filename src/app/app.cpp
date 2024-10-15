#include <trielo/trielo.hpp>
#include <cmsis_os2.h>
#include "example_submodule/public.hpp"
#include "util.hpp"
#include "app/app.hpp"

void producer(void* arg) {
    while(true) {
        std::printf("producer: *reinterpret_cast<*uint32_t>(arg): %lu\n\r", (*reinterpret_cast<uint32_t*>(arg))++);
        osDelay(100);
    }
}

void consumer(void* arg) {
    while(true) {
        std::printf("consumer: *reinterpret_cast<*uint32_t>(arg): %lu\n\r", (*reinterpret_cast<uint32_t*>(arg))--);
        osDelay(500);
    }
}

void launch_tasks(uint32_t* product) {
    const osThreadAttr_t producer_attr = {
        .name = "producer",
        .stack_size = 1024,
        .priority = (osPriority_t) osPriorityNormal,
    };
    osThreadNew(producer, reinterpret_cast<void*>(product), &producer_attr);

    const osThreadAttr_t consumer_attr = {
        .name = "consumer",
        .stack_size = 1024,
        .priority = (osPriority_t) osPriorityNormal,
    };
    osThreadNew(consumer, reinterpret_cast<void*>(product), &consumer_attr);
}

// This function cannot exit.
void app_main(void* arg) {
    (void) arg;
    start_watchdog();
    redirect_printf();

    Trielo::trielo<example_submodule::foo>();
    uint32_t product { 0 };
    launch_tasks(&product);
    while(1) {
        osThreadYield();
    }
}