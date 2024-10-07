#include <trielo/trielo.hpp>
#include "cmsis_os2.h"
#include "clk/clk.hpp"
#include "i2c2/common.hpp"
#include "i2c2/sw.hpp"

namespace i2c2 {
    namespace sw {
        Bus::Bus() {
            clk::init_i2c2();
            HAL_GPIO_WritePin(
                const_cast<GPIO_TypeDef*>(PORT),
                static_cast<uint16_t>(Pins::SDA) | static_cast<uint16_t>(Pins::SCL),
                GPIO_PIN_RESET
            );

            const GPIO_InitTypeDef GPIO_InitStruct {
                .Pin = static_cast<uint16_t>(Pins::SDA) | static_cast<uint16_t>(Pins::SCL),
                .Mode = GPIO_MODE_OUTPUT_OD,
                .Pull = GPIO_NOPULL,
                .Speed = GPIO_SPEED_FREQ_LOW,
            };

            HAL_GPIO_Init(const_cast<GPIO_TypeDef*>(PORT), const_cast<GPIO_InitTypeDef*>(&GPIO_InitStruct));
            set_SDA();
            set_SCL();
        }

        Bus::~Bus() {
            HAL_GPIO_DeInit(const_cast<GPIO_TypeDef*>(PORT), static_cast<uint16_t>(Pins::SCL));
            HAL_GPIO_DeInit(const_cast<GPIO_TypeDef*>(PORT), static_cast<uint16_t>(Pins::SDA));
        }

        void Bus::test_SCL() {
            for(uint8_t i = 0; i < 8; i++) {
                reset_SCL();
                osDelay(1000);
                set_SCL();
                osDelay(1000);
            }
        }

        void Bus::test_SDA() {
            for(uint8_t i = 0; i < 8; i++) {
                reset_SDA();
                osDelay(1000);
                set_SDA();
                osDelay(1000);
            }
        }

        void Bus::set_SDA() {
            HAL_GPIO_WritePin(const_cast<GPIO_TypeDef*>(PORT), static_cast<uint16_t>(Pins::SDA), GPIO_PIN_SET);
        }

        void Bus::reset_SDA() {
            HAL_GPIO_WritePin(const_cast<GPIO_TypeDef*>(PORT), static_cast<uint16_t>(Pins::SDA), GPIO_PIN_RESET);
        }

        void Bus::set_SCL() {
            HAL_GPIO_WritePin(const_cast<GPIO_TypeDef*>(PORT), static_cast<uint16_t>(Pins::SCL), GPIO_PIN_SET);
        }

        void Bus::reset_SCL() {
            HAL_GPIO_WritePin(const_cast<GPIO_TypeDef*>(PORT), static_cast<uint16_t>(Pins::SCL), GPIO_PIN_RESET);
        }

        void Bus::delay() {
            osDelay(100);
        }

        void Bus::start_cond() {
            set_SCL();
            set_SDA();
            delay();
            reset_SDA();
            delay();
            reset_SCL();
        }

        void Bus::stop_cond() {
            reset_SDA();
            delay();
            set_SCL();
            delay();
            set_SDA();
            delay();
        }

        void Bus::write_bit(const bool bit) {
            if(bit == true) {
                set_SDA();
            } else {
                reset_SDA();
            }

            delay();
            set_SCL();
            delay();
            reset_SCL();
        }

        bool Bus::read_SDA() {
            if(HAL_GPIO_ReadPin(const_cast<GPIO_TypeDef*>(PORT), static_cast<uint16_t>(Pins::SDA)) == GPIO_PIN_SET) {
                return true;
            } else {
                return false;
            }
        }

        bool Bus::read_bit() {
            set_SDA();
            delay();
            set_SCL();
            delay();
            const bool ret { read_SDA() };
            reset_SCL();
            return ret;
        }

        bool Bus::write_byte(const std::bitset<8>& byte, const bool start, const bool stop) {
            if(start) {
                start_cond();
            }

            // std::bitset doesn't come up with std::iterator implemented iterates over the bitset from the back to the front
            for(size_t i = byte.size() - 1; ;i--) {
                write_bit(byte[i]);
                if(i == 0) {
                    break;
                }
            }

            bool ack { read_bit() };

            if(stop == true) {
                stop_cond();
            }

            return !ack;
        }

        bool Bus::send_byte(uint8_t address, uint8_t data) {
            if(write_byte(address, true, false)) {
                if(write_byte(data, false, true)) {
                    return true;
                }
            }

            stop_cond();
            return false;
        }

        uint8_t Bus::recv_byte(uint8_t address) {
            if(write_byte((address << 1) | 0x01, true, false)) {
                return read_byte(false, true).to_ulong();
            }

            return 0;
        }

        std::bitset<8> Bus::read_byte(const bool ack, const bool stop) {
            std::bitset<8> ret {};
            for(size_t i = 0; i < ret.size(); i++) {
                ret = read_bit();
            }

            if(ack) {
                write_bit(true);
            } else {
                write_bit(false);
            }

            if(stop) {
                stop_cond();
            }

            return ret;
        }

        HAL_StatusTypeDef Bus::transmit(const uint16_t address, const std::span<uint8_t>& data) {
            if(write_byte(static_cast<uint8_t>(address & 0b0000'0000'1111'1110), true, false)) {
                for(size_t i = 0; i < data.size(); i++) {
                    if(i == data.size() - 1) {
                        if(write_byte(data[i], false, true)) {
                            return HAL_ERROR;
                        }
                    } else {
                        if(!write_byte(data[i], false, false)) {
                            break;
                        }
                    }
                }
            }

            stop_cond();
            return HAL_OK;
        }

        HAL_StatusTypeDef Bus::receive(const uint16_t address, const std::span<uint8_t>& data) {
            if(write_byte(address | 0x0001, true, false)) {
                for(size_t i = 0; i < data.size(); i++) {
                    data[i] = static_cast<uint8_t>(read_byte(false, false).to_ulong());
                }
            }

            stop_cond();
            return HAL_OK;
        }

        HAL_StatusTypeDef Bus::is_device_ready(const uint16_t address) {
            if(write_byte(static_cast<uint8_t>(address), true, true)) {
                return HAL_OK;
            }
            return HAL_ERROR;
        }

        void Bus::scan() {
            std::printf("Scanning I2C bus...\n\r");
            // I2C addresses are 7 bits, so they range from 0x08 to 0x77
            for(uint8_t address = 0x60; address < 0xFF; address++) {
                // Attempt to communicate with the device
                if(is_device_ready(address << 1) == HAL_OK
                    //HAL_I2C_IsDeviceReady(&hi2c2, (address << 1), 1, TIMEOUT) == HAL_OK
                ) {
                    // If the device responds, print its address
                    std::printf("Device found at 0x%02X\n\r", address);
                    const uint8_t ds3231_address { 0x68 };
                    if(address == ds3231_address) {
                        std::printf("address == ds3231_address\n\r");
                    }
                }
            }
            std::printf("I2C scan complete.\n\r");
        }
    }
}