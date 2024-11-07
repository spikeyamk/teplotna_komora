#pragma once

#include <iostream>
#include <array>
#include <bitset>

namespace sens {
namespace sht31 {
    class TempHum {
    private:
        static constexpr float temp_factor { 175.0f / 65535.0f };
        static constexpr float hum_factor  { 100.0f / 65535.0f };
        float temp;
        float hum;
    public:
        TempHum(const std::array<std::bitset<8>, 6>& serialized);
    public:
        float get_temp() const;
        float get_hum() const;
    };
}
}

std::ostream& operator<<(std::ostream& os, const sens::sht31::TempHum& temp_hum);