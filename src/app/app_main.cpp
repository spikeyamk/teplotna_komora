#include <array>
#include <bitset>
#include <string_view>
#include <limits>
#include <trielo/trielo.hpp>

#include "cmsis_os2.h"
#include "iwdg.h"

#include "comm/usb_uart/usb_uart.hpp"
#include "bksram/bksram.hpp"
#include "tasks/rs232_uart.hpp"

bool hw_test() {
    return true;
}

/**
 * @brief App entry point. This function cannot exit.
 * @param arg is necessary in oder for app_main's function pointer to be of type osThreadFunc_t. Remains unused, nullptr is injected into it. DO NOT DEREFERENCE!
 */
extern "C" void app_main(void* arg) {
    (void) arg;

    if(bksram::test() == false) {
        std::printf("app_main: bksram::test() == false: bksram::read().unwrap(): 0x%02lX\n", bksram::read().unwrap());
        __disable_irq();
        while(1) {
            HAL_IWDG_Refresh(&hiwdg);
        }
    }

    if(hw_test() == false) {
        bksram::write_reset<bksram::ErrorCodes::HWTEST>();
    }

    if(comm::usb_uart::RedirectStdout::get_instance().init() == false) {
        comm::usb_uart::RedirectStdout::get_instance().turn_off_threadsafe();
        std::printf("app_main: redirect_stdout.init() == false\n");
    }

    TRIELO(tasks::RS232_UART::get_instance().launch());

    for(uint32_t tick = 0; true; tick++) {
        //std::printf("app_main: tick: %lu\n", tick);
        osDelay(5'000);
    }

    // we should never get here...
}