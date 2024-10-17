#include <iostream>
#include "cmsis_os2.h"
#include "producer_consumer_test.hpp"

void produce(void* arg) {
    uint32_t* product { reinterpret_cast<uint32_t*>(arg) };
    while(1) {
        std::printf("produce: product: %lu\n", (*product)--);
        osDelay(500);
    }
}

void consume(void* arg) {
    uint32_t* product { reinterpret_cast<uint32_t*>(arg) };
    while(1) {
        std::printf("consume: product: %lu\n", (*product)++);
        osDelay(150);
    }
}

void launch_producer_consumer_test(uint32_t& product) {
    const osThreadAttr_t producer_attr {
        .name = "producer",
        .stack_size = 4 * 1024,
        .priority = (osPriority_t) osPriorityNormal,
    };
    osThreadNew(produce, &product, &producer_attr);
    const osThreadAttr_t consumer_attr {
        .name = "consumer",
        .stack_size = 4 * 1024,
        .priority = (osPriority_t) osPriorityNormal,
    };
    osThreadNew(consume, &product, &consumer_attr);
}