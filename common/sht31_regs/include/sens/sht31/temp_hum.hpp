#pragma once

#include <iostream>
#include <array>
#include <bitset>
#include <cstdint>

namespace sens {
namespace sht31 {
    class TempHum {
    public:
        uint16_t temp_raw;
        uint16_t hum_raw;
    public:
        TempHum(const uint16_t temp_raw, const uint16_t hum_raw) :
            temp_raw { temp_raw },
            hum_raw { hum_raw }
        {}

        TempHum(const std::array<std::bitset<8>, 6>& serialized);
    public:
        int32_t calculate_temp_celsius_multiplied_by_one_thousand() const;
        int32_t calculate_hum_percent_multiplied_by_one_thousand() const;
        float calculate_temp() const;
        float calculate_hum() const;
    };
}
}

std::ostream& operator<<(std::ostream& os, const sens::sht31::TempHum& temp_hum);