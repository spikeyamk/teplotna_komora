#include <iostream>
#include "cmsis_os2.h"
#include "ds3231/ds3231.hpp"
#include "ds3231/test.hpp"

namespace ds3231 {
    void test() {
        scan_i2c_bus(&hi2c2);
        for(size_t i = 0; true; i++) {
            print_time(
                ds3231_read_time(&hi2c2)
                .value_or(
                    std::array<uint8_t, 7>{
                        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
                    }
                )
            );  // Display the time
            if(i % 1000 == 0) {
                std::printf("tick: %u\n\r", i);
            }
            osDelay(1'000);
        }
    }
}