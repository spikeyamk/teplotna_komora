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
    private:
        Masks::Configuration::FaultDetection::WriteAction::Or fault_detection_write_action { Masks::Configuration::FaultDetection::WriteAction::Or::NO_ACTION };
    public:
        const Masks::Configuration::FaultDetection::ReadMeaning::Or fault_detection_read_meaning { Masks::Configuration::FaultDetection::ReadMeaning::Or(static_cast<uint8_t>(fault_detection_write_action)) };
        Masks::Configuration::FaultStatusAutoClear::Or fault_status_auto_clear { Masks::Configuration::FaultStatusAutoClear::Or::NOCLEAR };
    private:
        Masks::Configuration::FilterSelect::Or filter_select { Masks::Configuration::FilterSelect::Or::SIXTY_HZ };
    public:
        Configuration() = default;

        Configuration(
            const Masks::Configuration::Vbias::Or vbias,
            const Masks::Configuration::ConversionMode::Or conversion_mode,
            const Masks::Configuration::WireMode::Or wire_mode,
            const Masks::Configuration::FaultStatusAutoClear::Or fault_status_auto_clear
        ) :
            vbias { vbias },
            conversion_mode { conversion_mode },
            wire_mode { wire_mode },
            fault_detection_read_meaning { Masks::Configuration::FaultDetection::ReadMeaning::Or(static_cast<uint8_t>(fault_detection_write_action)) },
            fault_status_auto_clear { fault_status_auto_clear }
        {}

        Configuration(const std::bitset<8>& serialized);
    public:
        std::bitset<8> serialize() const;
        bool operator==(const Configuration& other) const;
        bool operator!=(const Configuration& other) const;

        friend class Extension;
    };
}
}