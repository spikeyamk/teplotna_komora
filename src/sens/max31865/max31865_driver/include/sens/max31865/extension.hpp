#pragma once

#include "sens/max31865/configuration.hpp"
#include "sens/max31865/fault_threshold.hpp"
#include "sens/max31865/rtd.hpp"
#include "sens/max31865/fault_status.hpp"
#include "sens/max31865/transceiver.hpp"

namespace sens {
namespace max31865 {
    class Extension {
    public:
        Transceiver& transceiver;
    public:
        Extension(Transceiver& transceiver) :
            transceiver { transceiver }
        {}
    public:
        HAL_StatusTypeDef dump();
        HAL_StatusTypeDef configure(const Configuration& configuration, const FaultThreshold& fault_threshold) const;
        HAL_StatusTypeDef configure_clear() const;
        std::expected<bool, HAL_StatusTypeDef> is_configured_against(const Configuration& configuration, const FaultThreshold& fault_threshold) const;
        std::expected<bool, HAL_StatusTypeDef> is_clear_configured() const;
        HAL_StatusTypeDef write_configuration(const Configuration& configuration) const;
        std::expected<Configuration, HAL_StatusTypeDef> read_configuration() const;
        HAL_StatusTypeDef configure_notch_filter_frequency(const Masks::Configuration::FilterSelect::Or filter_select) const;
        std::expected<RTD, HAL_StatusTypeDef> read_rtd() const;
        HAL_StatusTypeDef configure_auto_fault_detection() const;
        std::expected<FaultStatus, HAL_StatusTypeDef> read_fault_status() const;
    private:
        HAL_StatusTypeDef configure_notch_filter_frequency_unsafe(const Masks::Configuration::FilterSelect::Or filter_select, Configuration configuration_before) const;
    };
}
}