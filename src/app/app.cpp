#include <iostream>

#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"

#include "iwdg.h"
#include "tim.h"
#include "bksram/bksram.hpp"
#include "util/util.hpp"
#include "comm/usb_uart/usb_uart.hpp"

#include "tasks/senser_killer.hpp"
#include "tasks/temp_ctl.hpp"
#include "tasks/rs232_uart.hpp"
#include "tasks/panel.hpp"

extern "C" void vApplicationIdleHook(void) {
    __HAL_TIM_SET_COUNTER(&htim6, 0);
    HAL_IWDG_Refresh(&hiwdg);
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName) {
    std::printf("vApplicationStackOverflowHook: xTask: %p, pcTaskName: %s\n", (void*) xTask, pcTaskName);
}

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
extern "C" void MX_FREERTOS_Init() {
    if(bksram::test() == false) {
        util::shutdown_endless_loop();
    }

    if(comm::usb_uart::RedirectStdout::get_instance().init_threadsafe() == false) {
        comm::usb_uart::RedirectStdout::get_instance().turn_off_threadsafe();
        std::printf("MX_FREERTOS_Init: comm::usb_uart::RedirectStdout::get_instance().init_threadsafe() == false\n");
    }
    
    __HAL_TIM_SET_COUNTER(&htim6, 0);
    __HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(&htim6);

    tasks::SenserKiller::get_instance().init();
    if(tasks::SenserKiller::get_instance().launch() == false) {
        bksram::write_reset<bksram::ErrorCodes::SenserKiller::LAUNCH>();
    }

    if(tasks::TempCtl::get_instance().launch() == false) {
        bksram::write_reset<bksram::ErrorCodes::TempCtl::LAUNCH>();
    }

    if(tasks::Panel::get_instance().launch() == false) {
        bksram::write_reset<bksram::ErrorCodes::Panel::LAUNCH>();
    }

    tasks::RS232_UART::get_instance().init();
    if(tasks::RS232_UART::get_instance().launch() == false) {
        bksram::write_reset<bksram::ErrorCodes::RS232_UART::LAUNCH>();
    }
}