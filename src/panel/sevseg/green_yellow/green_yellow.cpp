#include <iostream>
#include <array>
#include "main.h"
#include "spi.h"
#include "panel/sevseg/common/common.hpp"
#include "panel/sevseg/green_yellow/green_yellow.hpp"

namespace panel {
namespace sevseg {
namespace green_yellow {
    MAX6549::MAX6549() {
        normal_operation_disable_blink_global_intensity_clear_all();
        turn_off_decode_mode();
        set_digit_7_to_0_to_7seg_or_16seg_type();
        set_max_scan_limit();
        set_max_global_intensity();
        clear_all();
    }

    void MAX6549::select() {
        HAL_GPIO_WritePin(SPI2_SEVYG_NSS_GPIO_Port, SPI2_SEVYG_NSS_Pin, GPIO_PIN_RESET);
    }

    void MAX6549::deselect() {
        HAL_GPIO_WritePin(SPI2_SEVYG_NSS_GPIO_Port, SPI2_SEVYG_NSS_Pin, GPIO_PIN_SET);
    }

    auto MAX6549::write(const uint8_t address, const uint8_t value) {
        select();
        const std::array<uint8_t, 2> buf { (address & 0x7F), value };
        const auto ret { HAL_SPI_Transmit(&hspi2, buf.data(), buf.size(), 1000) };
        deselect();
        return ret;
    }

    uint8_t MAX6549::read(const uint8_t address) {
        select();

        const std::array<uint8_t, 2> buf { (address | 0x80), 0x00 };
        HAL_SPI_Transmit(&hspi2, buf.data(), buf.size(), 1000);
        deselect();

        select();
        const std::array<uint8_t, 2> buf0 {};
        std::array<uint8_t, 2> ret {};
        HAL_SPI_TransmitReceive(&hspi2, buf0.data(), ret.data(), ret.size(), 1000);
        deselect();
        for(uint8_t i = 0; i < ret.size(); i++) {
            std::printf("ret[%u]: 0x%02X\n", i, ret[i]);
        }

        return ret[0];
    }

    void MAX6549::write_log(const uint8_t address, const uint8_t value) {
        std::printf("write(0x%02X, 0x%02X): 0x%02X\n", address, value, write(address, value));
    }

    void MAX6549::write_read_log(const uint8_t address, const uint8_t value) {
        std::printf("write(0x%02X, 0x%02X): 0x%02X\n", address, value, write(address, value));
        std::printf("read(0x%02X): 0x%02X\n", address, read(address));
    }

    void MAX6549::read_log(const uint8_t address) {
        std::printf("read(0x%02X): 0x%02X\n", address, read(address));
    }

    void MAX6549::clear_all() {
        for(uint8_t i = 0x20; i <= 0x2F; i++) {
            write_log(i, 0x00);
        }

        for(uint8_t i = 0x40; i <= 0x4F; i++) {
            write_log(i, 0x00);
        }
        
        for(uint8_t i = 0x60; i <= 0x6F; i++) {
            write_log(i, 0x00);
        }
    }

    void MAX6549::test_single_segment() {
        for(const auto map: common::single_segment_map) {
            for(uint8_t segment_address = 0x20; segment_address <= 0x2F; segment_address++) {
                write_log(segment_address, static_cast<uint8_t>(map.to_ulong()));
            }
            HAL_Delay(5'000);
        }
    }

    void MAX6549::test_hex() {
        for(const auto map: common::hex_map) {
            for(uint8_t segment_address = 0x20; segment_address <= 0x2F; segment_address++) {
                write_log(segment_address, static_cast<uint8_t>(map.to_ulong()));
            }
            HAL_Delay(5'000);
        }
    }

    void MAX6549::test_single_segment_single_digit() {
        for(const auto digit_address: digit_address_map) {
            for(const auto segment: common::single_segment_map) {
                write_log(digit_address, static_cast<uint8_t>(segment.to_ulong()));
                HAL_Delay(250);
            }
        }
    }

    void MAX6549::set_max_global_intensity() {
        set_intensity<0x0F>();
    }

    void MAX6549::normal_operation_disable_blink_global_intensity_clear_all() {
        write_log(CONFIGURATION, 0b0010'0001);
    }

    void MAX6549::turn_off_decode_mode() {
        write_log(DECODE_MODE, 0b0000'0000);
    }

    void MAX6549::set_max_scan_limit() {
        set_scan_limit<0x07>();
    }

    void MAX6549::set_digit_7_to_0_to_7seg_or_16seg_type() {
        write_log(DIGIT_TYPE, 0b0000'0000);
    }
    
    void MAX6549::shutdown() {
        write_log(CONFIGURATION, 0b0000'0000);
    }

    void MAX6549::show(const float value, const decltype(green_address_map)& address_map) {
        const auto sevmap { common::float_to_sevmap(value) };
        for(size_t i = 0; i < sevmap.size(); i++) {
            write(address_map[i], static_cast<uint8_t>(sevmap[i].to_ulong()));
        }
    }

    void MAX6549::yellow_show(const float value) {
        show(value, yellow_address_map);
    }

    void MAX6549::green_show(const float value) {
        show(value, green_address_map);
    }

    MAX6549::~MAX6549() {
        clear_all();
        shutdown();
    }

    void test() {
        while(1) {
            MAX6549 max6549 {};
            max6549.test_single_segment();
            max6549.test_hex();
            max6549.test_single_segment_single_digit();
            HAL_Delay(1000);
        }
    }
}
}
}