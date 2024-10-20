#include <iostream>
#include "cmsis_os.h"
#include "producer_consumer_test.hpp"

void produce(void* arg) {
    uint32_t& product { *reinterpret_cast<uint32_t*>(arg) };
    while(1) {
        std::printf("produce: product: %lu\r\n", product++);
        osDelay(1);
    }
}

void consume(void* arg) {
    uint32_t& product { *reinterpret_cast<uint32_t*>(arg) };
    while(1) {
        std::printf("consume: product: %lu\r\n", product--);
        //osDelay(1);
    }
}

void launch_producer_consumer_test(uint32_t& product) {
    static uint32_t producer_buffer[2048];
    static StaticTask_t producer_control_block;
    const osThreadAttr_t producer_attr {
        .name = "producer",
        .cb_mem = &producer_control_block,
        .cb_size = sizeof(producer_control_block),
        .stack_mem = &producer_buffer[0],
        .stack_size = sizeof(producer_buffer),
        .priority = (osPriority_t) osPriorityNormal,
    };
    osThreadNew(produce, &product, &producer_attr);

    static uint32_t consumer_buffer[2048];
    static StaticTask_t consumer_control_block;
    const osThreadAttr_t consumer_attr {
        .name = "consumer",
        .cb_mem = &consumer_control_block,
        .cb_size = sizeof(consumer_control_block),
        .stack_mem = &consumer_buffer[0],
        .stack_size = sizeof(consumer_buffer),
        .priority = (osPriority_t) osPriorityNormal,
    };
    osThreadNew(consume, &product, &consumer_attr);
}