#include "i2c.h"
#include "cmsis_os2.h"
#include "sens/sht31/sht31.hpp"

namespace sens {
namespace sht31 {
    int test() {
        Transceiver transceiver(&hi2c1, SlaveAddress7bit::ADDR_PIN_HIGH);
        Extension extension(transceiver);

        const auto ret_init { extension.init() };
        if(ret_init.has_value() == false) {
            std::printf("sens::sht31::test: ret_init.has_value() == false: ret_init.error(): 0x%02X\n", ret_init.error());
            return 1;
        }
        if(ret_init.value() == false) {
            std::printf("sens::sht31::test: ret_init.value() == false: ret_init.value(): %d\n", ret_init.value());
            return 2;
        }

        const auto ret_clear_status { extension.clear_status() };
        if(ret_clear_status != HAL_OK) {
            std::printf("sens::sht31::test: ret_clear_status != HAL_OK: ret_clear_status: 0x%02X\n", ret_clear_status);
            return 3;
        }

        const auto ret_read_status { extension.read_status() };
        if(ret_read_status.has_value() == false) {
            std::printf("sens::sht31::test: ret_read_status.has_value() == false: ret_read_status.error(): 0x%02X\n", ret_read_status.error());
            return 4;
        }

        if(ret_read_status.value() != sens::sht31::Status(std::bitset<16>(0x00'00))) {
            std::cout
                << "sens::sht31::test: ret_read_status.value() != sens::sht31::Status(std::bitset<16>(0x00'00): ret_read_status.value(): "
                << ret_read_status.value()
                << std::endl;
            return 5;
        }

        const auto ret_enable_heater { extension.enable_heater() };
        if(ret_enable_heater != HAL_OK) {
            std::printf("sens::sht31::test: ret_enable_heater != HAL_OK: ret_enable_heater: 0x%02X\n", ret_enable_heater);
            return 6;
        }

        const auto ret_read_status_after_enable_heater { extension.read_status() };
        if(ret_read_status_after_enable_heater.has_value() == false) {
            std::printf("sens::sht31::test: ret_read_status_after_enable_heater.has_value() == false: ret_read_status_after_enable_heater.error(): 0x%02X\n", ret_read_status_after_enable_heater.error());
            return 7;
        }

        if(ret_read_status_after_enable_heater.value() != sens::sht31::Status(std::bitset<16>(static_cast<uint16_t>(Masks::Status::Heater::Or::ON)))) {
            std::cout
                << "sens::sht31::test: ret_read_status_after_enable_heater.value() != sens::sht31::Status(std::bitset<16>(static_cast<uint16_t>(Masks::Status::Heater::Or::ON))): ret_read_status_after_enable_heater.value(): "
                << ret_read_status_after_enable_heater.value()
                << std::endl;
            return 8;
        }

        const auto ret_disable_heater { extension.disable_heater() };
        if(ret_disable_heater != HAL_OK) {
            std::printf("sens::sht31::test: ret_disable_heater != HAL_OK: ret_disable_heater: 0x%02X\n", ret_disable_heater);
            return 9;
        }

        const auto ret_read_status_after_disable_heater { extension.read_status() };
        if(ret_read_status_after_disable_heater.has_value() == false) {
            std::printf("sens::sht31::test: ret_read_status_after_disable_heater.has_value() == false: ret_read_status_after_disable_heater.error(): 0x%02X\n", ret_read_status_after_disable_heater.error());
            return 10;
        }

        if(ret_read_status_after_disable_heater.value() != sens::sht31::Status(std::bitset<16>(static_cast<uint16_t>(Masks::Status::Heater::Or::OFF)))) {
            std::cout
                << "sens::sht31::test: ret_read_status_after_disable_heater.value() != sens::sht31::Status(std::bitset<16>(static_cast<uint16_t>(Masks::Status::Heater::Or::OFF))): ret_read_status_after_disable_heater.value(): "
                << ret_read_status_after_disable_heater.value()
                << std::endl;
            return 11;
        }

        const auto ret_start_periodic_mode_ten_hertz_high_repeatability { extension.start_periodic_mode_ten_hertz_high_repeatability() };
        if(ret_start_periodic_mode_ten_hertz_high_repeatability != HAL_OK) {
            std::printf("sens::sht31::test: ret_start_periodic_mode_ten_hertz_high_repeatability != HAL_OK: 0x%02X\n", ret_start_periodic_mode_ten_hertz_high_repeatability);
            return 12;
        }

        osDelay(1'000);

        const auto ret_read_temp_hum_periodic_mode { extension.read_temp_hum_periodic_mode() };
        if(ret_read_temp_hum_periodic_mode.has_value() == false) {
            std::printf("sens::sht31::test: ret_read_temp_hum_periodic_mode.has_value() == false: ret_read_temp_hum_periodic_mode.error(): 0x%02X\n", ret_read_temp_hum_periodic_mode.error());
            return 13;
        }

        std::printf("sens::sht31::test: ret_read_temp_hum_periodic_mode.value().get_temp(): %f, ret_read_temp_hum_periodic_mode.value().get_hum(): %f\n", ret_read_temp_hum_periodic_mode.value().get_temp(), ret_read_temp_hum_periodic_mode.value().get_hum());

        if(extension.disable_heater() != HAL_OK) {
            std::printf("extension.disable_heater() != HAL_OK\n");
            return 14;
        }

        for(int32_t tick = 15; true; tick = (tick < 15 ? 15 : tick + 1)) {
            const auto temp_hum { extension.read_temp_hum_periodic_mode() };
            if(temp_hum.has_value() == false) {
                std::printf("sens::sht31::test: temp_hum.has_value() == false: temp_hum.error(): 0x%02X\n", temp_hum.error());
                return tick;
            }
            std::printf("sens::sht31::test: tick: %ld: temp: %f: hum: %f\n", tick, temp_hum.value().get_temp(), temp_hum.value().get_hum());
            osDelay(1'000);
        }

        return 0;
    }
}
}