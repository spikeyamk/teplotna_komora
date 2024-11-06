#include <cstdint>

#include "sens/max31865/configuration.hpp"

namespace sens {
namespace max31865 {
    Configuration::Configuration(const std::bitset<8>& serialized) :
        vbias {
            Masks::Configuration::Vbias::Or(
                static_cast<uint8_t>(
                    (
                        Masks::Configuration::Vbias::AND
                        & serialized
                    ).to_ulong()
                )
            )
        },
        conversion_mode { 
            Masks::Configuration::ConversionMode::Or(
                static_cast<uint8_t>(
                    (
                        Masks::Configuration::ConversionMode::AND
                        & serialized
                    ).to_ulong()
                )
            )
        },
        wire_mode {
            Masks::Configuration::WireMode::Or(
                static_cast<uint8_t>(
                    (
                        Masks::Configuration::WireMode::AND
                        & serialized
                    ).to_ulong()
                )
            )
        }
    {}

    std::bitset<8> Configuration::serialize() const {
        return {
            std::bitset<8>(static_cast<uint8_t>(vbias))
            | std::bitset<8>(static_cast<uint8_t>(conversion_mode))
            | std::bitset<8>(static_cast<uint8_t>(wire_mode))
        };
    }

    bool Configuration::operator==(const Configuration& other) const {
        return (
            (vbias == other.vbias)
            && (conversion_mode == other.conversion_mode)
            && (wire_mode == other.wire_mode)
        );
    }

    bool Configuration::operator!=(const Configuration& other) const {
        return !(*this == other);
    }
}
}