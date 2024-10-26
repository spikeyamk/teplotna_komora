#include <iostream>
#include <array>
#include <bitset>
#include <expected>
#include <optional>
#include <span>
#include <algorithm>
#include <string_view>

#include <ubitint.hpp>
#include "stm32f2xx_hal.h"
#include "cmsis_os2.h"

#include "sens/spi_temp/spi_temp.hpp"
#include "gpio.h"
#include "spi.h"

namespace sens {
namespace spi_temp {
    namespace select {
        void a() {
            HAL_GPIO_WritePin(SPI3_TEMP_NSS0_GPIO_Port, SPI3_TEMP_NSS0_Pin, GPIO_PIN_RESET);
        }

        void b() {
            HAL_GPIO_WritePin(SPI3_TEMP_NSS1_GPIO_Port, SPI3_TEMP_NSS1_Pin, GPIO_PIN_RESET);
        }
    }

    namespace deselect {
        void a() {
            HAL_GPIO_WritePin(SPI3_TEMP_NSS0_GPIO_Port, SPI3_TEMP_NSS0_Pin, GPIO_PIN_SET);
        }

        void b() {
            HAL_GPIO_WritePin(SPI3_TEMP_NSS1_GPIO_Port, SPI3_TEMP_NSS1_Pin, GPIO_PIN_SET);
        }
    }

    namespace write {
        auto a(const uint8_t address, const uint8_t value) {
            select::a();
            //HAL_Delay(10);
            const std::array<uint8_t, 2> buf { (address | 0x80), value };
            const auto ret { HAL_SPI_Transmit(&hspi3, buf.data(), buf.size(), 500) };
            deselect::a();
            return ret;
        }

        auto b(const uint8_t address, const uint8_t value) {
            select::b();
            //HAL_Delay(10);
            const std::array<uint8_t, 2> buf { (address | 0x80), value };
            const auto ret { HAL_SPI_Transmit(&hspi3, buf.data(), buf.size(), 500) };
            deselect::b();
            return ret;
        }
    }

    namespace read {
        uint8_t a(const uint8_t address) {
            select::a();
            //HAL_Delay(10);
            const uint8_t buf { (address & 0x7F) };
            HAL_SPI_Transmit(&hspi3, &buf, sizeof(buf), 500);
            uint8_t ret { 0x00 };
            HAL_SPI_Receive(&hspi3, &ret, sizeof(ret), 500);
            deselect::a();
            return ret;
        }

        uint8_t b(const uint8_t address) {
            select::b();
            //HAL_Delay(10);
            const uint8_t buf { (address & 0x7F) };
            HAL_SPI_Transmit(&hspi3, &buf, sizeof(buf), 500);
            uint8_t ret { 0x00 };
            HAL_SPI_Receive(&hspi3, &ret, sizeof(ret), 500);
            deselect::b();
            return ret;
        }
    }

    namespace init {
        static const std::array<uint8_t, 2> defaults_config_reset_fault { 0x00, 0xC2 };
        void a() {
            select::a();
            HAL_Delay(10);
            HAL_SPI_Transmit(&hspi3, defaults_config_reset_fault.data(), defaults_config_reset_fault.size(), 500);
            deselect::a();
        }

        void b() {
            select::b();
            HAL_Delay(10);
            HAL_SPI_Transmit(&hspi3, defaults_config_reset_fault.data(), defaults_config_reset_fault.size(), 500);
            deselect::b();
        }
    }

    namespace dump {
        void a() {
            select::a();

            const uint8_t buf { 0x00 };
            HAL_SPI_Transmit(&hspi3, &buf, sizeof(buf), 500);
            std::array<uint8_t, 8> ret_regs {};
            for(auto& e: ret_regs) {
                HAL_SPI_Receive(&hspi3, &e, sizeof(e), 500);
            }
            for(uint8_t i = 0; i < ret_regs.size(); i++) {
                std::printf("max31865::dump::a: ret_regs[%u]: 0x%02X\n", i, ret_regs[i]);
            }

            deselect::a();
        }

        void b() {
            select::b();

            const uint8_t buf { 0x00 };
            HAL_SPI_Transmit(&hspi3, &buf, sizeof(buf), 500);
            std::array<uint8_t, 8> ret_regs {};
            for(auto& e: ret_regs) {
                HAL_SPI_Receive(&hspi3, &e, sizeof(e), 500);
            }
            for(uint8_t i = 0; i < ret_regs.size(); i++) {
                std::printf("max31865::dump::b: ret_regs[%u]: 0x%02X\n", i, ret_regs[i]);
            }

            deselect::b();
        }
    }

    void test() {
        init::a();
        HAL_Delay(1000);
        init::b();
        HAL_Delay(1000);

        dump::a();
        dump::b();
        // For sensor A
        std::printf(
            "read::a(0x00): 0x%02X\n", 
            read::a(0x00)
        );

        std::printf(
            "write::a(0x00, 0xC2): 0x%02X\n", 
            write::a(0x00, 0xC2)
        );
        std::printf(
            "read::a(0x00): 0x%02X\n", 
            read::a(0x00)
        );

        std::printf(
            "write::a(0x00, 0x40): 0x%02X\n", 
            write::a(0x00, 0x40)
        );
        std::printf(
            "read::a(0x00): 0x%02X\n", 
            read::a(0x00)
        );


        // For sensor B
        std::printf(
            "read::b(0x00): 0x%02X\n", 
            read::b(0x00)
        );

        std::printf(
            "write::b(0x00, 0xC2): 0x%02X\n", 
            write::b(0x00, 0xC2)
        );
        std::printf(
            "read::b(0x00): 0x%02X\n", 
            read::b(0x00)
        );

        std::printf(
            "write::b(0x00, 0x40): 0x%02X\n", 
            write::b(0x00, 0x40)
        );
        std::printf(
            "read::b(0x00): 0x%02X\n", 
            read::b(0x00)
        );
    }
}
}