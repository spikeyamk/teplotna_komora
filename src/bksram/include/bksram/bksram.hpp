#pragma once

#include <cstdint>
#include <ubitint.hpp>
#include "stm32f2xx_hal.h"

#include "util/util.hpp"

namespace bksram {
    using uint20_t = ubitint_t<20>;
    using ErrorCodes = ::util::Registry<uint20_t,
        // fan
        0xE'F0'01,

        // temp_spi
        0xE'50'01,
        0xE'50'02,
        0xE'50'03,
        0xE'50'04,
        0xE'50'05,
        0xE'50'06,
        0xE'50'07,
        0xE'50'08,
        0xE'50'09,
        0xE'50'10,
        0xE'50'11,
        0xE'50'12
    >;

    template<uint20_t value>
    inline void write_reset() {
        HAL_PWR_EnableBkUpAccess();
        HAL_PWREx_EnableBkUpReg();
        __HAL_RCC_BKPSRAM_CLK_ENABLE();

        *reinterpret_cast<uint32_t*>(BKPSRAM_BASE) = ErrorCodes::get<value>().unwrap();
        NVIC_SystemReset();
    }

    inline ubitint_t<20> read() {
        HAL_PWR_EnableBkUpAccess();
        HAL_PWREx_EnableBkUpReg();
        __HAL_RCC_BKPSRAM_CLK_ENABLE();

        return *reinterpret_cast<uint32_t*>(BKPSRAM_BASE);
    }
}