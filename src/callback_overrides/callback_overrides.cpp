#include "stm32f2xx_hal.h"
#include "main.h"
#include "panel/encoder/encoder.hpp"
#include "actu/fan/fb/fb.hpp"
#include "comm/usb_uart/usb_uart.hpp"
#include "bksram/bksram.hpp"
#include "tasks/senser_killer.hpp"
#include "tasks/rs232_uart.hpp"
#include "panel/button/button.hpp"

extern "C" int __io_putchar(int ch) {
    return comm::usb_uart::RedirectStdout::get_instance().transmit(ch);
}

extern "C" int _write(int file, char *ptr, int len) {
    (void)file;

    if(xPortIsInsideInterrupt() == false) {
        comm::usb_uart::RedirectStdout::get_instance().acquire_mutex();
    }

    for(int i = 0; i < len; i++) {
        if(xPortIsInsideInterrupt()) {
            const HAL_StatusTypeDef ret_err { comm::usb_uart::RedirectStdout::get_instance().transmit(*ptr++) };
            if(ret_err != HAL_OK) {
                return ret_err;
            }
        } else if(comm::usb_uart::RedirectStdout::get_instance().push(*ptr++) == false) {
            comm::usb_uart::RedirectStdout::get_instance().flush();
            comm::usb_uart::RedirectStdout::get_instance().push(*ptr++);
        }
    }

    if(xPortIsInsideInterrupt() == false) {
        comm::usb_uart::RedirectStdout::get_instance().flush();
        comm::usb_uart::RedirectStdout::get_instance().release_mutex();
    }

    return len;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    switch(GPIO_Pin) {
        case ENCA_EXTI10_Pin:
            panel::encoder::enca_exti10_handler();
            break;
        case ENCB_EXTI11_Pin:
            panel::encoder::encb_exti11_handler();
            break;
        case BUT0_FR_Pin:
            panel::button::but0_fr_handler();
            break;
        case BUT1_MR_Pin:
            panel::button::but1_mr_handler();
            break;
        case BUT2_ML_Pin:
            panel::button::but2_ml_handler();
            break;
        case BUT3_FL_Pin:
            panel::button::but3_fl_handler();
            break;
        case SPI3_TEMP_NDRDY0_Pin:
            tasks::SenserKiller::get_instance().release_semaphore_front();
            break;
        case SPI3_TEMP_NDRDY1_Pin:
            tasks::SenserKiller::get_instance().release_semaphore_rear();
            break;
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    actu::fan::fb::HAL_TIM_IC_CaptureCallback(htim);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
    actu::fan::fb::HAL_TIM_PWM_PulseFinishedCallback(htim);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM1) {
        HAL_IncTick();
    } else if(htim->Instance == TIM6) {
        std::printf("HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim): htim->Instance == TIM6: TWDG did not stop!\n");
        bksram::write<bksram::ErrorCodes::TWDG::EXPIRE>();
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if(huart->Instance == USART3) {
        tasks::RS232_UART::get_instance().release_semaphore(Size);
    }
}