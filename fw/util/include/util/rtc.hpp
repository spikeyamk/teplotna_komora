#pragma once

#include <array>
#include "stm32f2xx_hal.h"

namespace util {
namespace rtc {
    HAL_StatusTypeDef set_time(const uint8_t hr, const uint8_t min, const uint8_t sec);
    HAL_StatusTypeDef set_date(const uint8_t year, const uint8_t month, const uint8_t date, const uint8_t weekday);
    HAL_StatusTypeDef get_time_date(std::array<char, 100>& time, std::array<char, 100>& date);
    void get_time_date_test();
}
}