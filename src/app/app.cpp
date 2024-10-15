#include <trielo/trielo.hpp>
#include <cmsis_os2.h>
#include "example_submodule/public.hpp"
#include "util.hpp"
#include "main.h"
#include "mfrc522/MFRC522.hpp"
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
    //start_watchdog();
    redirect_printf();

    HAL_GPIO_WritePin(SPI4_NRSTPD_GPIO_Port, SPI4_NRSTPD_Pin, GPIO_PIN_SET);
    osDelay(1000);

    MFRC522_Reset();
    osDelay(1000);

    Write_MFRC522(TModeReg, 0x80);
    osDelay(1000);
    std::printf(
        "Read_MFRC522(TModeReg): 0x%02X\n\r",
        Read_MFRC522(TModeReg)
    );
    osDelay(1000);

    Write_MFRC522(TPrescalerReg, 0xA9); //0x34);
    osDelay(1000);
    std::printf(
        "Read_MFRC522(TPrescalerReg): 0x%02X\n\r",
        Read_MFRC522(TPrescalerReg)
    );
    osDelay(1000);

    Write_MFRC522(TReloadRegL, 0x03);
    osDelay(1000);
    std::printf(
        "Read_MFRC522(TReloadRegL): 0x%02X\n\r",
        Read_MFRC522(TReloadRegL)
    );
    osDelay(1000);

    Write_MFRC522(TReloadRegH, 0xE8);
    osDelay(1000);
    std::printf(
        "Read_MFRC522(TReloadRegH): 0x%02X\n\r",
        Read_MFRC522(TReloadRegH)
    );
    osDelay(1000);

    Write_MFRC522(TxAutoReg, 0x40);
    osDelay(1000);
    std::printf(
        "Read_MFRC522(TxAutoReg): 0x%02X\n\r",
        Read_MFRC522(TxAutoReg)
    );
    osDelay(1000);

    Write_MFRC522(ModeReg, 0x3D);
    osDelay(1000);
    std::printf(
        "Read_MFRC522(ModeReg): 0x%02X\n\r",
        Read_MFRC522(ModeReg)
    );
    osDelay(1000);

    while(1) {
        osThreadYield();
    }
}