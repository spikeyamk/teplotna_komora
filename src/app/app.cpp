#include <array>
#include <string_view>
#include <limits>
#include <trielo/trielo.hpp>
#include "example_submodule/public.hpp"
#include "actu/fan/fan.hpp"
#include "actu/bridge/bridge.hpp"
#include "actu/buzzer/buzzer.hpp"
#include "actu/lin_source/lin_source.hpp"
#include "actu/pump/pump.hpp"
#include "panel/sevseg/white/white.hpp"
#include "panel/sevseg/green_yellow/green_yellow.hpp"
#include "sens/i2c/common/common.hpp"
//#include "sens/spi_temp/spi_temp.hpp"
#include "stm32f2xx_hal.h"
#include "main.h"
#include "app/app.hpp"
#include "dac.h"
#include "gpio.h"
#include "spi.h"

namespace max6549 {
    void select() {
        HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_RESET);
    }

    void deselect() {
        HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);
    }

    auto write(const uint8_t address, const uint8_t value) {
        select();
        const std::array<uint8_t, 2> buf { ((address) & 0x7E), value };
        const auto ret { HAL_SPI_Transmit(&hspi2, buf.data(), buf.size(), 1000) };
        deselect();
        return ret;
    }

    uint8_t read(const uint8_t address) {
        select();
        const uint8_t buf { ((address) | 0x80) };
        HAL_SPI_Transmit(&hspi2, &buf, sizeof(buf), 1000);
        uint8_t ret { 0x00 };
        HAL_SPI_Receive(&hspi2, &ret, sizeof(ret), 1000);
        deselect();
        return ret;
    }

    void test() {
        std::printf("write(0x01, (0x01 )): 0x%02X\n", write(0x01, (0x01 )));
        std::printf("read(0x01): 0x%02X\n", read(0x01));

        std::printf("write(0x02, (0xFF )): 0x%02X\n", write(0x02, (0xFF )));
        std::printf("read(0x02): 0x%02X\n", read(0x02));

        std::printf("write(0x03, (0x05 )): 0x%02X\n", write(0x03, (0xFF)));
        std::printf("read(0x03): 0x%02X\n", read(0x03));

        std::printf("write(0x04, (0x01 )): 0x%02X\n", write(0x04, (0x01 )));
        std::printf("read(0x04): 0x%02X\n", read(0x04));

        for(uint8_t i = 0x20; i <= 0x2F; i++) {
            std::printf("write(0x%02X, 0xFF): 0x%02X\n", i, write(i, (0xFF )));
            std::printf("read(0x%02X): 0x%02X\n", i, read(i));
        }

        for(uint8_t i = 0x40; i <= 0x4F; i++) {
            std::printf("write(0x%02X, 0xEF): 0x%02X\n", i, write(i, (0xFF )));
            std::printf("read(0x%02X): 0x%02X\n", i, read(i));
        }
    }

}

namespace max31865 {
    namespace select {
        void a() {
            HAL_GPIO_WritePin(SPI3_NSS1_GPIO_Port, SPI3_NSS0_Pin, GPIO_PIN_RESET);
        }

        void b() {
            HAL_GPIO_WritePin(SPI3_NSS0_GPIO_Port, SPI3_NSS1_Pin, GPIO_PIN_RESET);
        }
    }

    namespace deselect {
        void a() {
            HAL_GPIO_WritePin(SPI3_NSS0_GPIO_Port, SPI3_NSS0_Pin, GPIO_PIN_SET);
        }

        void b() {
            HAL_GPIO_WritePin(SPI3_NSS0_GPIO_Port, SPI3_NSS1_Pin, GPIO_PIN_SET);
        }
    }

    namespace write {
        auto a(const uint8_t address, const uint8_t value) {
            select::a();
            const std::array<uint8_t, 2> buf { (address | 0x80), value };
            const auto ret { HAL_SPI_Transmit(&hspi3, buf.data(), buf.size(), 500) };
            deselect::a();
            return ret;
        }

        auto b(const uint8_t address, const uint8_t value) {
            select::b();
            const std::array<uint8_t, 2> buf { (address | 0x80), value };
            const auto ret { HAL_SPI_Transmit(&hspi3, buf.data(), buf.size(), 500) };
            deselect::b();
            return ret;
        }
    }

    namespace read {
        uint8_t a(const uint8_t address) {
            select::a();
            const uint8_t buf { (address & 0x7E) };
            HAL_SPI_Transmit(&hspi3, &buf, sizeof(buf), 500);
            uint8_t ret { 0x00 };
            HAL_SPI_Receive(&hspi3, &ret, sizeof(ret), 500);
            deselect::a();
            return ret;
        }

        uint8_t b(const uint8_t address) {
            select::b();
            const uint8_t buf { (address & 0x7E) };
            HAL_SPI_Transmit(&hspi3, &buf, sizeof(buf), 500);
            uint8_t ret { 0x00 };
            HAL_SPI_Receive(&hspi3, &ret, sizeof(ret), 500);
            deselect::b();
            return ret;
        }
    }

    void test() {
        std::printf("write::a(0x00, 0x00): 0x%02X\n", write::a(0x00, 0x80));
        std::printf("read::a(0x00): 0x%02X\n", read::a(0x00));
    }
}

/// This function calculates the area of a rectangle.
void app_main(void* arg) {
    (void) arg;

    /* STM32H503x has 128K FLASH only these functions don't fit into it */
    //Trielo::trielo<submodule::foo>();

    actu::fan::init_tim();
    Trielo::trielo<actu::fan::init_ctl>();
    panel::sevseg::white::init_brightness();
    panel::sevseg::white::turn_on_all_segments();
    actu::pump::stop();
    actu::buzzer::stop();
    actu::fan::stop_all();

    actu::bridge::a::turn_off();
    actu::bridge::b::turn_off();
    actu::lin_source::start_dac();
    actu::lin_source::set_output(std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max());

    //actu::bridge::a::cool();
    //actu::bridge::b::cool();
    Trielo::trielo<max6549::test>();
    //Trielo::trielo<max31865::test>();
    for(
        uint16_t dac_value = 0;
        true;
        dac_value = [](const uint16_t in) {
            static constexpr uint16_t inc { 2 << 9 };
            static constexpr uint16_t stopper { 2 << 11 };
            return (in + inc) > stopper ? 0 : in + inc;
        }(dac_value)
    ) {
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value);
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac_value);
        std::printf("dac_value: %u\n", dac_value);
        HAL_Delay(5000);
    }
}