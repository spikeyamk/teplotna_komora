#include "sens/sht31/temp_hum.hpp"

namespace sens {
namespace sht31 {
    TempHum::TempHum(const std::array<std::bitset<8>, 6>& serialized) :
        temp_raw {
            static_cast<uint16_t>(
                (
                    (std::bitset<16>(serialized[0].to_ulong()) << 8)
                    | std::bitset<16>(serialized[1].to_ulong())
                ).to_ulong()
            )
        },
        hum_raw {
            static_cast<uint16_t>(
                (
                    (std::bitset<16>(serialized[2].to_ulong()) << 8)
                    | std::bitset<16>(serialized[3].to_ulong())
                ).to_ulong()
            )
        }
    {}

    int32_t TempHum::calculate_temp_celsius_multiplied_by_one_thousand() const {
        return (21875 * static_cast<int32_t>(temp_raw) >> 13) - 45000;
    }

    int32_t TempHum::calculate_hum_percent_multiplied_by_one_thousand() const {
        return (12500 * static_cast<int32_t>(hum_raw) >> 13);
    }

    float TempHum::calculate_temp() const {
        return static_cast<float>(calculate_temp_celsius_multiplied_by_one_thousand()) / 1'000.0f;
    }

    float TempHum::calculate_hum() const {
        return static_cast<float>(calculate_hum_percent_multiplied_by_one_thousand()) / 1'000.0f;
    }
}
}

std::ostream& operator<<(std::ostream& os, const sens::sht31::TempHum& temp_hum) {
    os
        << "TempHum:\n"
        << "  temp_hum.calculate_temp():" << temp_hum.calculate_temp() << "\n"
        << "  temp_hum.calculate_hum():" << temp_hum.calculate_hum() << "\n";
    return os;
}