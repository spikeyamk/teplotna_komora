#include <iostream>
#include <array>
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "stm32h7xx_nucleo.h"
#include "iwdg.h"

// Structure to track task status
struct TaskStatus {
    const char *taskName;       // Name of the task
    bool isAlive;               // Flag to check if the task is alive
    TickType_t lastCheck;       // Last time the task checked in
};

std::array<TaskStatus, 3> tasksStatus {};  // Array to hold task statuses

// Watchdog Task to monitor other tasks
void watchdog(void *argument) {
    (void) argument;
    const TickType_t xDelay = pdMS_TO_TICKS(500);    // Check interval (500 ms)
    const TickType_t taskTimeout = pdMS_TO_TICKS(2000); // Task inactivity timeout (2000 ms)

    while (1) {
        TickType_t currentTime = osKernelGetTickCount();  // Get current tick count

        // Check each task's status
        for (uint32_t i = 0; i < tasksStatus.size(); i++) {
            if (tasksStatus[i].isAlive && (currentTime - tasksStatus[i].lastCheck) > taskTimeout) {
                // Task has stalled
                tasksStatus[i].isAlive = false;

                // Print an error message with the task name
                std::printf("ERROR: Task '%s' has stalled or blocked too long!\n", tasksStatus[i].taskName);
            }
        }

        // Refresh the watchdog
        HAL_IWDG_Refresh(&hiwdg1);

        // Delay before the next check
        osDelay(xDelay);
    }
}

void start_watchdog() {
    const osThreadAttr_t watchdog_attr = {
        .name = "watchdog",
        .stack_size = 1024,
        .priority = (osPriority_t) osPriorityHigh7,
    };
    osThreadNew(watchdog, nullptr, &watchdog_attr);
}

void redirect_printf() {
    /* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
    COM_InitTypeDef BspCOMInit {
        .BaudRate   = 115200,
        .WordLength = COM_WORDLENGTH_8B,
        .StopBits   = COM_STOPBITS_1,
        .Parity     = COM_PARITY_NONE,
        .HwFlowCtl  = COM_HWCONTROL_NONE,
    };
    if(BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE) {
        Error_Handler();
    }
}