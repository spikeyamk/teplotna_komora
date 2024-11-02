#include "sens/max31865/filter_select.hpp"
#include "sens/max31865/extension.hpp"

namespace sens {
namespace max31865 {
    HAL_StatusTypeDef Extension::configure(const Configuration& configuration) const {
        const auto configuration_register_before { transceiver.read(RegAddrs::RO::CONFIGURATION) };

        if(configuration_register_before.has_value() == false) {
            return configuration_register_before.error();
        }

        const auto configuration_register_to_write {
            (configuration_register_before.value() & (~Masks::Configuration::AND)) | configuration.serialize()
        };

        return transceiver.write(RegAddrs::RW::CONFIGURATION, configuration_register_to_write);
    }

    HAL_StatusTypeDef Extension::set_fault_threshold(const FaultThreshold& fault_threshold) const {
        const auto fault_threshold_serialized { fault_threshold.serialize() };

        HAL_StatusTypeDef err_ret = transceiver.write(RegAddrs::RW::HIGH_FAULT_THRESHOLD_MSBS, fault_threshold_serialized[0]);
        if(err_ret != HAL_OK) {
            return err_ret;
        }

        err_ret = transceiver.write(RegAddrs::RW::HIGH_FAULT_THRESHOLD_LSBS, fault_threshold_serialized[1]);
        if(err_ret != HAL_OK) {
            return err_ret;
        }

        err_ret = transceiver.write(RegAddrs::RW::LOW_FAULT_THRESHOLD_MSBS, fault_threshold_serialized[2]);
        if(err_ret != HAL_OK) {
            return err_ret;
        }

        return transceiver.write(RegAddrs::RW::LOW_FAULT_THRESHOLD_LSBS, fault_threshold_serialized[3]);
    }

    std::expected<FaultThreshold, HAL_StatusTypeDef> Extension::read_fault_threshold() const {
        const auto ret_high_fault_threshold_msbs {
            transceiver.read(RegAddrs::RO::HIGH_FAULT_THRESHOLD_MSBS)
        };
        if(ret_high_fault_threshold_msbs.has_value() == false) {
            return std::unexpected(ret_high_fault_threshold_msbs.error());
        }

        const auto ret_high_fault_threshold_lsbs {
            transceiver.read(RegAddrs::RO::HIGH_FAULT_THRESHOLD_LSBS)
        };
        if(ret_high_fault_threshold_lsbs.has_value() == false) {
            return std::unexpected(ret_high_fault_threshold_lsbs.error());
        }

        const auto ret_low_fault_threshold_msbs {
            transceiver.read(RegAddrs::RO::LOW_FAULT_THRESHOLD_MSBS)
        };
        if(ret_low_fault_threshold_msbs.has_value() == false) {
            return std::unexpected(ret_low_fault_threshold_msbs.error());
        }

        const auto ret_low_fault_threshold_lsbs {
            transceiver.read(RegAddrs::RO::LOW_FAULT_THRESHOLD_LSBS)
        };
        if(ret_low_fault_threshold_lsbs.has_value() == false) {
            return std::unexpected(ret_low_fault_threshold_lsbs.error());
        }

        return FaultThreshold {{
            {
                ret_high_fault_threshold_msbs.value(),
                ret_high_fault_threshold_lsbs.value(),
                ret_low_fault_threshold_msbs.value(),
                ret_low_fault_threshold_lsbs.value(),
            }
        }};
    }

    HAL_StatusTypeDef Extension::set_filter_select(const Masks::FilterSelect::Or filter_select) const {
        const auto configuration_reg_before { transceiver.read(RegAddrs::RO::CONFIGURATION) };
        if(configuration_reg_before.has_value() == false) {
            return configuration_reg_before.error();
        }

        const auto configuration_reg_to_restore {
            (configuration_reg_before.value() & (~Masks::FilterSelect::AND))
            | std::bitset<8>(static_cast<uint8_t>(filter_select))
        };

        if(Configuration(configuration_reg_before.value()).conversion_mode == Masks::Configuration::ConversionMode::Or::AUTO) {
            const auto configuration_reg_before_without_auto_conversion_mode {
                (configuration_reg_before.value() & (~Masks::Configuration::ConversionMode::AND))
                | std::bitset<8>(static_cast<uint8_t>(filter_select))
            };
            
            const auto ret { transceiver.write(RegAddrs::RW::CONFIGURATION, configuration_reg_before_without_auto_conversion_mode) };
            if(ret != HAL_OK) {
                return ret;
            }
        }

        return transceiver.write(RegAddrs::RW::CONFIGURATION, configuration_reg_to_restore);
    }

    std::expected<Masks::FilterSelect::Or, HAL_StatusTypeDef> Extension::read_filter_select() const {
        const auto ret { transceiver.read(RegAddrs::RO::CONFIGURATION) };
        if(ret.has_value() == false) {
            return std::unexpected(ret.error());
        }
        
        return Masks::FilterSelect::Or(static_cast<uint8_t>((ret.value() & Masks::FilterSelect::AND).to_ulong()));
    }

    std::expected<RTD, HAL_StatusTypeDef> Extension::read_rtd() {
        if(semaphore == nullptr) {
            return std::unexpected(HAL_ERROR);
        }

        if(osSemaphoreAcquire(semaphore, semaphore_timeout) != osOK) {
            return std::unexpected(HAL_ERROR);
        }

        if(HAL_GPIO_ReadPin(ndrdy_port, ndrdy_pin) != GPIO_PIN_RESET) {
            return std::unexpected(HAL_ERROR);
        }

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

    osStatus Extension::release_semaphore() {
        if(semaphore == nullptr) {
            return osError;
        }
        return osSemaphoreRelease(semaphore);
    }

    std::expected<FaultStatus, HAL_StatusTypeDef> Extension::run_auto_fault_detection() const {
        const auto configuration_reg_before { transceiver.read(RegAddrs::RO::CONFIGURATION) };
        if(configuration_reg_before.has_value() == false) {
            return std::unexpected(configuration_reg_before.error());
        }

        const Configuration configuration_before { configuration_reg_before.value() };
        const FilterSelect filter_select_before { configuration_reg_before.value() };

        const auto configuration_reg_with_auto_fault_detection {
            std::bitset<8>(static_cast<uint8_t>(Masks::Configuration::Vbias::Or::ON))
            | std::bitset<8>(static_cast<uint8_t>(configuration_before.wire_mode))
            | std::bitset<8>(static_cast<uint8_t>(filter_select_before.filter_select))
        };

        const auto ret_write { transceiver.write(RegAddrs::RW::CONFIGURATION, configuration_reg_with_auto_fault_detection) };
        if(ret_write != HAL_OK) {
            return std::unexpected(ret_write);
        }

        HAL_Delay(1);

        const auto read_configuration_reg_after_auto_fault_detection_run { transceiver.read(RegAddrs::RO::CONFIGURATION) };
        if(read_configuration_reg_after_auto_fault_detection_run.has_value() == false) {
            return std::unexpected(read_configuration_reg_after_auto_fault_detection_run.error());
        }

        if(
            (
                read_configuration_reg_after_auto_fault_detection_run.value()
                & Masks::FaultDetection::ReadMeaning::AND
            )
            == std::bitset<8>(static_cast<uint8_t>(Masks::FaultDetection::ReadMeaning::Or::FAULT_DETECTION_FINISHED))
        ) {
            return std::unexpected(HAL_ERROR);
        }

        const auto read_fault_status { transceiver.read(RegAddrs::RO::FAULT_STATUS) };
        if(read_fault_status.has_value() == false) {
            return std::unexpected(read_fault_status.error());
        }

        const auto ret_restore_configuration_reg_before { transceiver.write(sens::max31865::RegAddrs::RW::CONFIGURATION, configuration_reg_before.value()) };
        if(ret_restore_configuration_reg_before != HAL_OK) {
            return std::unexpected(ret_restore_configuration_reg_before);
        }

        return FaultStatus(read_fault_status.value());
    }

    std::expected<FaultStatus, HAL_StatusTypeDef> Extension::read_fault_status() const {
        const auto ret { transceiver.read(RegAddrs::RO::FAULT_STATUS) };
        if(ret.has_value() == false) {
            return std::unexpected(ret.error());
        }

        return FaultStatus { ret.value() };
    }

    std::expected<Configuration, HAL_StatusTypeDef> Extension::read_configuration() const {
        const auto read_configuration_reg { transceiver.read(RegAddrs::RO::CONFIGURATION) };
        if(read_configuration_reg.has_value() == false) {
            return std::unexpected(read_configuration_reg.error());
        }

        return Configuration { read_configuration_reg.value() };
    }

    HAL_StatusTypeDef Extension::clear_fault_status() const {
        const auto read_configuration_reg { transceiver.read(RegAddrs::RO::CONFIGURATION) };
        if(read_configuration_reg.has_value() == false) {
            return read_configuration_reg.error();
        }

        const auto configuration_reg_to_write {
            read_configuration_reg.value()
            | std::bitset<8>(static_cast<uint8_t>(Masks::FaultStatusAutoClear::Or::CLEAR))
        };

        return transceiver.write(RegAddrs::RW::CONFIGURATION, configuration_reg_to_write);
    }
}
}