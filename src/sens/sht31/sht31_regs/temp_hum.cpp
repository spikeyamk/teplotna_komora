#include "sens/sht31/temp_hum.hpp"

namespace sens {
namespace sht31 {
    TempHum::TempHum(const std::array<std::bitset<8>, 6>& serialized) :
        temp {
            -45.0f
            + (
                temp_factor
                * static_cast<float>(
                    (
                        (std::bitset<16>(serialized[0].to_ulong()) << 8)
                        | std::bitset<16>(serialized[1].to_ulong())
                    ).to_ulong()
                )
            )
        },
        hum {
            hum_factor
            * static_cast<float>(
                (
                    (std::bitset<16>(serialized[3].to_ulong()) << 8)
                    | std::bitset<16>(serialized[4].to_ulong())
                ).to_ulong()
            )
        }
    {}

    float TempHum::get_temp() const {
        return temp;
    }

    float TempHum::get_hum() const {
        return hum;
    }
}
}

std::ostream& operator<<(std::ostream& os, const sens::sht31::TempHum& temp_hum) {
    os
        << "TempHum:\n"
        << "  temp_hum.get_temp():" << temp_hum.get_temp() << "\n"
        << "  temp_hum.get_hum():" << temp_hum.get_hum() << "\n";
    return os;
}