#include <iostream>
#include "cmsis_os2.h"
#include "ds3231/ds3231.hpp"
#include "i2c2/i2c2.hpp"
#include "ds3231/test.hpp"

namespace ds3231 {
    void test() {
        {
            /*
            i2c2::hw::Bus i2c2_hw_bus;
            i2c2_hw_bus.scan();
            DS3231 ds3231_hw { i2c2_hw_bus };
            ds3231_hw.print_time();
            */
        }
        {
            i2c2::sw::Bus i2c2_sw_bus;
            i2c2_sw_bus.scan();
            DS3231 ds3231_sw { i2c2_sw_bus };
            ds3231_sw.print_time();
        }
    }
}