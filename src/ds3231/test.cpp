#include <iostream>
#include "cmsis_os2.h"
#include "ds3231/ds3231.hpp"
#include "i2c2/i2c2.hpp"
#include "ds3231/test.hpp"

namespace ds3231 {
    void test() {
        i2c2::hw::Bus i2c2_hw_bus;
        DS3231 ds3231 { i2c2_hw_bus };
        ds3231.print_time();
    }
}