#include <array>

#include "sens/sht31/extension.hpp"

namespace sens {
namespace sht31 {
    std::expected<bool, HAL_StatusTypeDef> Extension::init() const {
        const auto ret_status { transceiver.read<3>() };
        if(ret_status.has_value() == false) {
            return std::unexpected(ret_status.error());
        }

        const std::array<std::bitset<8>, 2> data {
            ret_status.value()[0], 
            ret_status.value()[1],
        };

        return (transceiver.calculate_crc(data) == ret_status.value()[2]);
    }

    HAL_StatusTypeDef Extension::clear_status() const {
        return transceiver.write_command(Commands::Status::CLEAR);
    }

    std::expected<Status, HAL_StatusTypeDef> Extension::read_status() const {
        const auto ret_status { transceiver.read<3>() };
        if(ret_status.has_value() == false) {
            return std::unexpected(ret_status.error());
        }

        const std::array<std::bitset<8>, 2> data {
            ret_status.value()[0], 
            ret_status.value()[1],
        };

        if(transceiver.calculate_crc(data) != ret_status.value()[2]) {
            return std::unexpected(HAL_ERROR);
        }

        return Status { 
            (std::bitset<16>(ret_status.value()[0].to_ulong()) << 8)
            | std::bitset<16>(ret_status.value()[1].to_ulong())
        };
    }

    HAL_StatusTypeDef Extension::enable_heater() const {
        return transceiver.write_command(std::bitset<16>(static_cast<uint16_t>(Commands::Heater::ENABLE)));
    }

    HAL_StatusTypeDef Extension::disable_heater() const {
        return transceiver.write_command(std::bitset<16>(static_cast<uint16_t>(Commands::Heater::DISABLE)));
    }

    HAL_StatusTypeDef Extension::soft_reset() const {
        return transceiver.write_command(Commands::SOFT_RESET);
    }

    HAL_StatusTypeDef Extension::start_periodic_mode_ten_hertz(const Commands::Periodic::TEN_HERTZ repeatibility) const {
        return transceiver.write_command(std::bitset<16>(static_cast<uint16_t>(repeatibility)));
    }

    HAL_StatusTypeDef Extension::start_periodic_mode_one_hertz(const Commands::Periodic::ONE_HERTZ repeatibility) const {
        return transceiver.write_command(std::bitset<16>(static_cast<uint16_t>(repeatibility)));
    }

    HAL_StatusTypeDef Extension::stop_periodic_mode() const {
        return transceiver.write_command(Commands::Periodic::BREAK);
    }

    std::expected<TempHum, HAL_StatusTypeDef> Extension::read_temp_hum_periodic_mode() const {
        const auto err_ret { transceiver.write_command(Commands::Periodic::FETCH_DATA) };
        if(err_ret != HAL_OK) {
            return std::unexpected(err_ret);
        }

        const auto ret_read { transceiver.read<6>() };
        if(ret_read.has_value() == false) {
            return std::unexpected(ret_read.error());
        }

        if(
            Transceiver::calculate_crc(
                std::array<std::bitset<8>, 2> {
                    ret_read.value()[0],
                    ret_read.value()[1] 
                }
            )
            != ret_read.value()[2]
        ) {
            return std::unexpected(HAL_ERROR);
        }

        if(
            Transceiver::calculate_crc(
                std::array<std::bitset<8>, 2> {
                    ret_read.value()[3],
                    ret_read.value()[4] 
                }
            )
            != ret_read.value()[5]
        ) {
            return std::unexpected(HAL_ERROR);
        }

        return TempHum { ret_read.value() };
    }
}
}