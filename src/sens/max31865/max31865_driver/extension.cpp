#include "sens/max31865/extension.hpp"

namespace sens {
namespace max31865 {
    HAL_StatusTypeDef Extension::dump() {
        const auto ret_read_all { transceiver.read_all() };
        if(ret_read_all.has_value() == false) {
            return ret_read_all.error(); 
        }

        for(size_t i = 0; i < ret_read_all.value().size(); i++) {
            std::printf("sens::max31865::dump: this: %p, ret_read_all.value()[%zu]: 0x%02lX\n", reinterpret_cast<void*>(this), i, ret_read_all.value()[i].to_ulong());
        }

        return HAL_OK;
    }

    HAL_StatusTypeDef Extension::configure(const Configuration& configuration, const FaultThreshold& fault_threshold) const {
        HAL_StatusTypeDef err_ret { transceiver.write(RegAddrs::RW::CONFIGURATION, configuration.serialize()) };
        if(err_ret != HAL_OK) {
            return err_ret;
        }

        for(const auto& e: fault_threshold.serialize()) {
            err_ret = transceiver.write(RegAddrs::RW::HIGH_FAULT_THRESHOLD_MSBS, e);
            if(err_ret != HAL_OK) {
                return err_ret;
            }
        }

        return err_ret;
    }

    HAL_StatusTypeDef Extension::configure_clear() const {
        const Configuration clear_configuration {
            Masks::Configuration::Vbias::Or::OFF,
            Masks::Configuration::ConversionMode::Or::OFF,
            Masks::Configuration::WireMode::Or::TWO_WIRE_OR_FOUR_WIRE,
            Masks::Configuration::FaultStatusAutoClear::Or::CLEAR
        };

        return configure(clear_configuration, FaultThreshold());
    }


    std::expected<bool, HAL_StatusTypeDef> Extension::is_clear_configured() const {
        return is_configured_against(Configuration(), FaultThreshold());
    }

    HAL_StatusTypeDef Extension::write_configuration(const Configuration& configuration) const {
        return transceiver.write(RegAddrs::RW::CONFIGURATION, configuration.serialize());
    }

    std::expected<Configuration, HAL_StatusTypeDef> Extension::read_configuration() const {
        const auto read { transceiver.read(RegAddrs::RO::CONFIGURATION) };
        if(read.has_value() == false) {
            return std::unexpected(read.error());
        }

        return read.value();
    }

    HAL_StatusTypeDef Extension::configure_notch_filter_frequency(const Masks::Configuration::FilterSelect::Or filter_select) const {
        const auto configuration_before { read_configuration() };
        if(configuration_before.has_value() == false) {
            return configuration_before.error();
        }

        if(configuration_before.value().conversion_mode == Masks::Configuration::ConversionMode::Or::AUTO) {
            const Configuration configuration_before_without_auto_conversion_mode {
                [&]() {
                    auto ret { configuration_before.value() };
                    ret.conversion_mode = Masks::Configuration::ConversionMode::Or::OFF;
                    return ret;
                }()
            };

            const auto ret { configure_notch_filter_frequency_unsafe(filter_select, configuration_before_without_auto_conversion_mode) };
            if(ret != HAL_OK) {
                return ret;
            }

            return write_configuration(configuration_before.value());
        }

        return configure_notch_filter_frequency_unsafe(filter_select, configuration_before.value());
    }

    std::expected<RTD, HAL_StatusTypeDef> Extension::read_rtd() const {
        const auto ret_rtd_lsbs { transceiver.read(RegAddrs::RO::RTD_LSBS) };
        if(ret_rtd_lsbs.has_value() == false) {
            return std::unexpected(ret_rtd_lsbs.error());
        }
        
        const auto ret_rtd_msbs { transceiver.read(RegAddrs::RO::RTD_MSBS) };
        if(ret_rtd_msbs.has_value() == false) {
            return std::unexpected(ret_rtd_msbs.error());
        }

        return RTD { std::array<std::bitset<8>, 2> { ret_rtd_msbs.value(), ret_rtd_lsbs.value() } };
    }

    HAL_StatusTypeDef Extension::configure_notch_filter_frequency_unsafe(const Masks::Configuration::FilterSelect::Or filter_select, Configuration configuration_before) const { 
        configuration_before.filter_select = filter_select;
        return transceiver.write(RegAddrs::RW::CONFIGURATION, configuration_before.serialize());
    }

    HAL_StatusTypeDef Extension::configure_auto_fault_detection() const {
        const auto configuration_before { read_configuration() };
        if(configuration_before.has_value() == false) {
            return configuration_before.error();
        }

        const Configuration run_auto_fault_configuration {
            [&]() {
                Configuration ret {
                    Masks::Configuration::Vbias::Or::ON,
                    Masks::Configuration::ConversionMode::Or::OFF,
                    configuration_before.value().wire_mode,
                    Masks::Configuration::FaultStatusAutoClear::Or::NOCLEAR,
                };
                ret.fault_detection_write_action = Masks::Configuration::FaultDetection::WriteAction::Or::FAULT_DETECTION_WITH_AUTOMATIC_DELAY;

                return ret;
            }()
        };

        const auto ret_write { write_configuration(run_auto_fault_configuration) };
        if(ret_write != HAL_OK) {
            return ret_write;
        }

        HAL_Delay(10);
        return write_configuration(configuration_before.value());
    }

    std::expected<FaultStatus, HAL_StatusTypeDef> Extension::read_fault_status() const {
        const auto ret { transceiver.read(RegAddrs::RO::FAULT_STATUS) };
        if(ret.has_value() == false) {
            return std::unexpected(ret.error());
        }

        return FaultStatus { ret.value() };
    }

    std::expected<bool, HAL_StatusTypeDef> Extension::is_configured_against(const Configuration& configuration, const FaultThreshold& fault_threshold) const {
        const auto ret_read_all { transceiver.read_all() };
        if(ret_read_all.has_value() == false) {
            return std::unexpected(ret_read_all.error());
        }

        if(Configuration(ret_read_all.value()[static_cast<size_t>(RegAddrs::RO::CONFIGURATION)]) != configuration) {
            return false;
        }

        const std::array<std::bitset<8>, 4> ret_fault_threshold_serialized {
            ret_read_all.value()[static_cast<size_t>(RegAddrs::RO::HIGH_FAULT_THRESHOLD_MSBS)],
            ret_read_all.value()[static_cast<size_t>(RegAddrs::RO::HIGH_FAULT_THRESHOLD_LSBS)],
            ret_read_all.value()[static_cast<size_t>(RegAddrs::RO::LOW_FAULT_THRESHOLD_MSBS)],
            ret_read_all.value()[static_cast<size_t>(RegAddrs::RO::LOW_FAULT_THRESHOLD_LSBS)],
        };

        if(FaultThreshold(ret_fault_threshold_serialized) != fault_threshold) {
            return false;
        }

        return true;
    }
}
}