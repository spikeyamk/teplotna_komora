#include <trielo/trielo.hpp>

#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"

#include "bksram/bksram.hpp"
#include "util/util.hpp"
#include "tasks/tasks.hpp"

extern "C" void vApplicationIdleHook(void) {
    util::twdg_refresh();
    util::iwdg_refresh();
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
    if(Trielo::trielo<bksram::test>(Trielo::Success(true)) == false) {
        util::shutdown_endless_loop();
    }

    if(Trielo::trielo<util::twdg_init>(Trielo::Success(HAL_OK)) != HAL_OK) {
        bksram::write_reset<bksram::ErrorCodes::TWDG::INIT>();
    }

    if(tasks::SenserKiller::get_instance().launch() == false) {
        bksram::write_reset<bksram::ErrorCodes::SenserKiller::LAUNCH>();
    }

    if(tasks::Panel::get_instance().launch() == false) {
        bksram::write_reset<bksram::ErrorCodes::Panel::LAUNCH>();
    }
    
    if(tasks::SevsegWhite::get_instance().launch() == false) {
        bksram::write_reset<bksram::ErrorCodes::SevsegWhite::LAUNCH>();
    }

    if(tasks::RS232_UART::get_instance().launch() == false) {
        bksram::write_reset<bksram::ErrorCodes::RS232_UART::LAUNCH>();
    }

    if(tasks::TempCtl::get_instance().launch() == false) {
        bksram::write_reset<bksram::ErrorCodes::TempCtl::LAUNCH>();
    }
}