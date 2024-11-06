#pragma once

#include <cstdint>

#include "sens/max31865/masks.hpp"

namespace sens {
namespace max31865 {
    class Configuration {
    public:
        Masks::Configuration::Vbias::Or vbias { Masks::Configuration::Vbias::Or::OFF };
        Masks::Configuration::ConversionMode::Or conversion_mode { Masks::Configuration::ConversionMode::Or::OFF };
        Masks::Configuration::WireMode::Or wire_mode { Masks::Configuration::WireMode::Or::TWO_WIRE_OR_FOUR_WIRE };
    public:
        Configuration() = default;

        Configuration(
            const Masks::Configuration::Vbias::Or vbias,
            const Masks::Configuration::ConversionMode::Or conversion_mode,
            const Masks::Configuration::WireMode::Or wire_mode
        ) :
            vbias { vbias },
            conversion_mode { conversion_mode },
            wire_mode { wire_mode }
        {}

        Configuration(const std::bitset<8>& serialized);
    public:
        std::bitset<8> serialize() const;
        bool operator==(const Configuration& other) const;
        bool operator!=(const Configuration& other) const;
    };
}
}