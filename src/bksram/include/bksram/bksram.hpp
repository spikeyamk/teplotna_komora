#pragma once

#include <cstdint>
#include <ubitint.hpp>
#include "stm32f2xx_hal.h"

#include "util/util.hpp"

namespace bksram {
    using uint20_t = ubitint_t<20>;
    struct ErrorCodes {
        static constexpr uint20_t MISSING_LF { 0xE'01'11 };
        static constexpr uint20_t HWTEST     { 0xE'10'00 };
        static constexpr uint20_t TWDG       { 0xE'70'00 };

        using Registry = ::util::Registry<uint20_t,
            MISSING_LF,
            HWTEST,
            TWDG
        >;
    };

    template<uint20_t value>
    inline void write() {
        HAL_PWR_EnableBkUpAccess();
        HAL_PWREx_EnableBkUpReg();
        __HAL_RCC_BKPSRAM_CLK_ENABLE();

        *reinterpret_cast<uint32_t*>(BKPSRAM_BASE) = ErrorCodes::Registry::get<value>().unwrap();
    }

    template<uint20_t value>
    inline void write_reset() {
        write<value>();
        NVIC_SystemReset();
    }

    inline ubitint_t<20> read() {
        HAL_PWR_EnableBkUpAccess();
        HAL_PWREx_EnableBkUpReg();
        __HAL_RCC_BKPSRAM_CLK_ENABLE();

        return *reinterpret_cast<uint32_t*>(BKPSRAM_BASE);
    }

    bool test();
}