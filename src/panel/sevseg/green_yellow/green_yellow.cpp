#include <iostream>
#include <array>
#include "main.h"
#include "spi.h"
#include "panel/sevseg/green_yellow/green_yellow.hpp"

namespace panel {
namespace sevseg {
namespace green_yellow {
namespace max6549 {
    void select() {
        HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_RESET);
    }

    void deselect() {
        HAL_GPIO_WritePin(SPI2_NSS_GPIO_Port, SPI2_NSS_Pin, GPIO_PIN_SET);
    }

    auto write(const uint8_t address, const uint8_t value) {
        select();
        const std::array<uint8_t, 2> buf { (address & 0x7F), value };
        const auto ret { HAL_SPI_Transmit(&hspi2, buf.data(), buf.size(), 1000) };
        deselect();
        return ret;
    }

    uint8_t read(const uint8_t address) {
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

    void write_log(const uint8_t address, const uint8_t value) {
        std::printf("write(0x%02X, 0x%02X): 0x%02X\n", address, value, write(address, value));
    }

    void write_read_log(const uint8_t address, const uint8_t value) {
        std::printf("write(0x%02X, 0x%02X): 0x%02X\n", address, value, write(address, value));
        //std::printf("read(0x%02X): 0x%02X\n", address, read(address));
    }

    void read_log(const uint8_t address) {
        std::printf("read(0x%02X): 0x%02X\n", address, read(address));
    }



    class MAX6549 {
    private:
        static constexpr uint8_t DECODE_MODE { 0x01 };
        static constexpr uint8_t GLOBAL_INTENSITY { 0x02 };
        static constexpr uint8_t SCAN_LIMIT { 0x03 };
        static constexpr uint8_t CONFIGURATION { 0x04 };
        static constexpr uint8_t DIGIT_TYPE { 0x0C };

        static constexpr uint8_t YELLOW_0 { 0x20 };
        static constexpr uint8_t YELLOW_1 { 0x28 };
        static constexpr uint8_t YELLOW_2 { 0x21 };
        static constexpr uint8_t YELLOW_3 { 0x29 };
        static constexpr uint8_t YELLOW_4 { 0x22 };

        static constexpr uint8_t GREEN_0 { 0x24 };
        static constexpr uint8_t GREEN_1 { 0x2C };
        static constexpr uint8_t GREEN_2 { 0x25 };
        static constexpr uint8_t GREEN_3 { 0x2D };
        static constexpr uint8_t GREEN_4 { 0x26 };

        static constexpr std::array<uint8_t, 10> digit_address_map {
            YELLOW_0,
            YELLOW_1,
            YELLOW_2,
            YELLOW_3,
            YELLOW_4,
            GREEN_0,
            GREEN_1,
            GREEN_2,
            GREEN_3,
            GREEN_4,
        };

        static constexpr std::array<uint8_t, 8> single_segment_map {
            0b0000'0001, // stred
            0b0000'0010, // lavo hore
            0b0000'0100, // lavo dole
            0b0000'1000, // dole
            0b0001'0000, // vpravo dole
            0b0010'0000, // vpravo hore
            0b0100'0000, // hore
            0b1000'0000, // bodka
        };

        static constexpr std::array<uint8_t, 16> hex_map {
            0b0111'1110, // 0
            0b0011'0000, // 1
            0b0110'1101, // 2
            0b0111'1001, // 3
            0b0011'0011, // 4
            0b0101'1011, // 5
            0b0101'1111, // 6
            0b0111'0000, // 7
            0b0111'1111, // 8
            0b0111'1011, // 9
            0b0111'0111, // A
            0b0001'1111, // b
            0b0100'1110, // C
            0b0011'1101, // d
            0b0100'1111, // E
            0b0100'0111  // F
        };
    public:
        MAX6549() {
            normal_operation_disable_blink_global_intensity_clear_all();
            turn_off_decode_mode();
            set_digit_7_to_0_to_7seg_or_16seg_type();
            set_max_scan_limit();
            set_max_global_intensity();
            clear_all();
        }

        void clear_all() {
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

        void test_single_segment() {
            for(const auto map: single_segment_map) {
                for(uint8_t segment_address = 0x20; segment_address <= 0x2F; segment_address++) {
                    write_log(segment_address, map);
                }
                HAL_Delay(5'000);
            }
        }

        void test_hex() {
            for(const auto map: hex_map) {
                for(uint8_t segment_address = 0x20; segment_address <= 0x2F; segment_address++) {
                    write_log(segment_address, map);
                }
                HAL_Delay(5'000);
            }
        }

        void test_single_segment_single_digit() {
            for(const auto digit_address: digit_address_map) {
                for(const auto segment: single_segment_map) {
                    write_log(digit_address, segment);
                    HAL_Delay(250);
                }
            }
        }

        void set_max_global_intensity() {
            set_intensity<0x0F>();
        }

        template<uint8_t level>
        void set_intensity() {
            static_assert(level <= 0x0F, "MAX6549::set_intensity: level must be less than or equal to 0x0F");
            write_log(GLOBAL_INTENSITY, level);
        }

        void normal_operation_disable_blink_global_intensity_clear_all() {
            write_log(CONFIGURATION, 0b0010'0001);
        }

        void turn_off_decode_mode() {
            write_log(DECODE_MODE, 0b0000'0000);
        }

        void set_max_scan_limit() {
            set_scan_limit<0x07>();
        }

        template<uint8_t digits>
        void set_scan_limit() {
            static_assert(digits <= 0x07, "MAX6549::set_scan_limit: digits must be less than or equal to 0x07");
            write_log(SCAN_LIMIT, digits);
        }

        void set_digit_7_to_0_to_7seg_or_16seg_type() {
            write_log(DIGIT_TYPE, 0b0000'0000);
        }
        
        void shutdown() {
            write_log(CONFIGURATION, 0b0000'0000);
        }

        ~MAX6549() {
            clear_all();
            shutdown();
        }
    };

    void test() {
        while(1) {
            MAX6549 max6549 {};
            //max6549.test_single_segment();
            //max6549.test_hex();
            max6549.test_single_segment_single_digit();
            HAL_Delay(1000);
        }
    }
}
}
}
}