#pragma once

#include "stm32h7xx_hal.h"

inline void enable_backup_sram() {
    HAL_PWR_EnableBkUpAccess();
    HAL_PWREx_EnableBkUpReg();
    __HAL_RCC_BKPRAM_CLK_ENABLE();
}

inline void writeBkpSram(const uint32_t magic) {
    enable_backup_sram();
    *reinterpret_cast<uint32_t*>(D3_BKPSRAM_BASE) = magic;
}

inline uint32_t readBkpSram() {
    enable_backup_sram();
    return *reinterpret_cast<uint32_t*>(D3_BKPSRAM_BASE);
}

void redirect_printf();