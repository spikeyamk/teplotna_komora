#include <iostream>
#include "ds3231/ds3231.hpp"
#include "i2cbb/i2cbb.hpp"
#include "ds3231/test.hpp"
#include "peripheral/gpio/i2cbb.hpp"

namespace ds3231 {
    void test() {
        i2cbb::Bus i2cbb_bus {
            peripheral::gpio::i2cbb::PORT,
            static_cast<uint16_t>(peripheral::gpio::i2cbb::Pins::I2CBB1_SDA),
            static_cast<uint16_t>(peripheral::gpio::i2cbb::Pins::I2CBB1_SCL)
        };
        i2cbb_bus.scan();
        DS3231 ds3231 { i2cbb_bus };
        ds3231.print_time();
    }
}