#pragma once

#include "stm32f2xx_hal.h"
#include "iwdg.h"
#include "tim.h"
#include "cmsis_os2.h"

namespace util {
    inline void microsec_blocking_delay(const uint32_t us) {
        const uint32_t SYSTICK_LOAD { SystemCoreClock / 1000000U };
        const uint32_t SYSTICK_DELAY_CALIB { SYSTICK_LOAD >> 1 };

        do {
            const uint32_t start = SysTick->VAL;
            const uint32_t ticks = (us * SYSTICK_LOAD)-SYSTICK_DELAY_CALIB;
            while((start - SysTick->VAL) < ticks);
        } while (0);
    }
}

namespace util {
    void turn_every_annoying_peripheral_off();
    void shutdown_endless_loop();
    HAL_StatusTypeDef twdg_init();
    inline void twdg_refresh() {
        __HAL_TIM_SET_COUNTER(&htim6, 0);
    }

    inline void iwdg_refresh() {
        HAL_IWDG_Refresh(&hiwdg);
    }
}