#pragma once

#include <cstdint>
#include <ubitint.hpp>
#include "stm32h7xx_hal.h"
#include "util/util.hpp"

namespace bksram {
    using uint20_t = ubitint_t<20>;

    // Define the error codes as 20-bit unsigned integers
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

    // Function template to write to BKPSRAM
    template<uint20_t value>
    inline void write() {
        // Enable access to the backup domain
        HAL_PWR_EnableBkUpAccess();
        // Enable the Backup SRAM regulator for retention
        HAL_PWREx_EnableBkUpReg();
        // Enable the clock for BKPSRAM
        __HAL_RCC_BKPRAM_CLK_ENABLE();

        // Write the value to the Backup SRAM
        *reinterpret_cast<uint32_t*>(D3_BKPSRAM_BASE) = ErrorCodes::Registry::get<value>().unwrap();
    }

    // Function template to write to BKPSRAM and reset the system
    template<uint20_t value>
    inline void write_reset() {
        write<value>();
        NVIC_SystemReset();  // Reset the system
    }

    // Function to read from BKPSRAM
    inline ubitint_t<20> read() {
        // Enable access to the backup domain
        HAL_PWR_EnableBkUpAccess();
        // Enable the Backup SRAM regulator for retention
        HAL_PWREx_EnableBkUpReg();
        // Enable the clock for BKPSRAM
        __HAL_RCC_BKPRAM_CLK_ENABLE();

        // Read the value from the Backup SRAM
        return *reinterpret_cast<uint32_t*>(D3_BKPSRAM_BASE);
    }

    // Function to test the Backup SRAM functionality
    bool test();
}
