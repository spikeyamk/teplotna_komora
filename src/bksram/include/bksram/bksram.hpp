#pragma once

#include <cstdint>
#include "stm32f2xx_hal.h"

namespace bksram {
    inline void write(const uint32_t value) {
        HAL_PWR_EnableBkUpAccess();
        HAL_PWREx_EnableBkUpReg();
        __HAL_RCC_BKPSRAM_CLK_ENABLE();

        *reinterpret_cast<uint32_t*>(BKPSRAM_BASE) = value;
    }

    inline uint32_t read() {
        HAL_PWR_EnableBkUpAccess();
        HAL_PWREx_EnableBkUpReg();
        __HAL_RCC_BKPSRAM_CLK_ENABLE();

        return *reinterpret_cast<uint32_t*>(BKPSRAM_BASE);
    }
}