// https://github.com/PascalPolygon/stm32_bitbang_i2c

#include <iostream>
#include "i2cbb/common.hpp"
#include "i2cbb/i2cbb.hpp"

namespace i2cbb {
    Bus::Bus(const GPIO_TypeDef* port, const uint16_t sda, const uint16_t scl) :
        port { port },
        sda { sda },
        scl { scl }
    {
        set_SDA();
        set_SCL();
    }

    Bus::~Bus() {
        reset_SCL();
        reset_SDA();
    }

    void Bus::set_SCL() {
        HAL_GPIO_WritePin(const_cast<GPIO_TypeDef*>(port), static_cast<uint16_t>(scl), GPIO_PIN_SET);
    }

    void Bus::set_SDA() {
        HAL_GPIO_WritePin(const_cast<GPIO_TypeDef*>(port), static_cast<uint16_t>(sda), GPIO_PIN_SET);
    }

    void Bus::reset_SCL() {
        HAL_GPIO_WritePin(const_cast<GPIO_TypeDef*>(port), static_cast<uint16_t>(scl), GPIO_PIN_RESET);
    }

    void Bus::reset_SDA() {
        HAL_GPIO_WritePin(const_cast<GPIO_TypeDef*>(port), static_cast<uint16_t>(sda), GPIO_PIN_RESET);
    }

    void Bus::start_cond() {
        set_SCL();
        set_SDA();
        delay();
        reset_SDA();
        delay();
        reset_SCL();
        delay();
    }

    void Bus::stop_cond() {
        delay();
        reset_SDA();
        delay();
        set_SCL();
        delay();
        set_SDA();
        delay();
    }

    void Bus::write_bit(const uint8_t b) {
        if(b > 0)
            set_SDA();
        else
            reset_SDA();

        delay();
        set_SCL();
        delay();
        reset_SCL();
    }

    uint8_t Bus::read_SDA() {
        if(HAL_GPIO_ReadPin(const_cast<GPIO_TypeDef*>(port), sda) == GPIO_PIN_SET)
            return 1;
        else
            return 0;
    }

    // Reading a bit in Bus:
    uint8_t Bus::read_bit() {
        set_SDA();
        delay();
        set_SCL();
        delay();

        const uint8_t b { Bus::read_SDA() };
        reset_SCL();
        return b;
    }

    bool Bus::write_byte(uint8_t B, const bool start, const bool stop) {
        if(start)
            Bus::start_cond();

        for(uint8_t i = 0; i < 8; i++) {
            Bus::write_bit(B & 0x80); // write the most-significant bit
            B <<= 1;
        }

        uint8_t ack = Bus::read_bit();

        if(stop)
            Bus::stop_cond();

        return !ack; //0-ack, 1-nack
    }

    // Reading a byte with Bus:
    uint8_t Bus::read_byte(const bool ack, const bool stop)
    {
        uint8_t B = 0;

        for(uint8_t i = 0; i < 8; i++) {
            B <<= 1;
            B |= Bus::read_bit();
        }

        if(ack)
            Bus::write_bit(0);
        else
            Bus::write_bit(1);

        if(stop)
            Bus::stop_cond();

        return B;
    }

    // Sending a byte with Bus:
    bool Bus::send_byte(const uint8_t address, const uint8_t data) {
        //    if( Bus::write_byte( address << 1, true, false ) )   // start, send address, write
        if(Bus::write_byte(address, true, false)) {// start, send address, write
            // send data, stop
            if(Bus::write_byte(data, false, true))
                return true;
        }

        Bus::stop_cond(); // make sure to impose a stop if NAK'd
        return false;
    }

    // Receiving a byte with Bus:
    uint8_t Bus::receive_byte(const uint8_t address) {
        if(Bus::write_byte((address << 1) | 0x01, true, false)) {// start, send address, read
            return Bus::read_byte(false, true);
        }

        return 0; // return zero if NAK'd
    }

    // Sending a byte of data with Bus:
    bool Bus::send_byte_data(const uint8_t address, const uint8_t reg, const uint8_t data) {
        //    if( Bus::write_byte( address << 1, true, false ) )   // start, send address, write
        if(Bus::write_byte(address, true, false)) {
            if(Bus::write_byte(reg, false, false)) {// send desired register
                if(Bus::write_byte(data, false, true))
                    return true; // send data, stop
            }
        }

        Bus::stop_cond();
        return false;
    }

    // Receiving a byte of data with Bus:
    uint8_t Bus::receive_byte_data(const uint8_t address, const uint8_t reg) {
        //if( Bus::write_byte( address << 1, true, false ) )   // start, send address, write
        if(Bus::write_byte(address, true, false)) {
            if(Bus::write_byte(reg, false, false)) {// send desired register
                if(Bus::write_byte((address << 1) | 0x01, true, false)) // start again, send address, read
                    return Bus::read_byte(false, true); // read data
            }
        }

        Bus::stop_cond();
        return 0; // return zero if NACKed
    }

    HAL_StatusTypeDef Bus::transmit(const uint8_t address, const std::span<uint8_t>& data) {
        if(Bus::write_byte(address & 0xFE, true, false)) {// first byte
            for(uint8_t i = 0; i < data.size(); i++) {
                if(i == data.size() - 1) {
                    if(Bus::write_byte(data[i], false, true))
                        return HAL_OK;
                } else if(!Bus::write_byte(data[i], false, false))
                    break; //last byte
            }
        }

        Bus::stop_cond();
        return HAL_ERROR;
    }
    
    HAL_StatusTypeDef Bus::is_device_ready(const uint8_t address) {
        /*
        if(Bus::write_byte(address & 0xFE, true, true)) {// first byte
            return HAL_OK;
            for(uint8_t i = 0; i < data.size(); i++) {
                if(i == data.size() - 1) {
                    if(Bus::write_byte(data[i], false, true))
                } else if(!Bus::write_byte(data[i], false, false))
                    break; //last byte
            }
        }
        */

        Bus::stop_cond();
        return HAL_ERROR;
    }

    HAL_StatusTypeDef Bus::receive(const uint8_t address, const std::span<uint8_t>& data) {
        if(Bus::write_byte(address | 0x01, true, false)) {// start again, send address, read (LSB signifies R or W)
            for(uint8_t i = 0; i < data.size(); i++) {
                data[i] = Bus::read_byte(true, false); // read data
            }
            Bus::stop_cond();
            return HAL_OK;
        }
        Bus::stop_cond();
        return HAL_ERROR;
    }

    void Bus::scan() {
        std::printf("Scanning I2C bus...\n\r");
        // I2C addresses are 7 bits, so they range from 0x08 to 0x77
        std::array<uint8_t, 1> reg { 0xFF };
        for(uint8_t address = 0x00; address < 0xFF; address++) {
            // Attempt to communicate with the device
            if(transmit((address << 1), reg) == HAL_OK) {
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