#include <array>

#include "sens/sht31/addrs.hpp"
#include "sens/sht31/transceiver.hpp"

namespace sens {
namespace sht31 {
    Transceiver::Transceiver(I2C_HandleTypeDef* hi2c, const SlaveAddress slave_address) :
        hi2c { hi2c },
        slave_address {
            static_cast<uint16_t>((std::bitset<16>(static_cast<uint16_t>(slave_address)) << 1).to_ulong())
        }
    {}

    HAL_StatusTypeDef Transceiver::write_command(const std::bitset<16>& command) const {
        std::array<uint8_t, 2> buf {
            static_cast<uint8_t>((command >> 8).to_ulong()),
            static_cast<uint8_t>((command & std::bitset<16>(0x00'FF)).to_ulong()),
        };
        return HAL_I2C_Master_Transmit(hi2c, slave_address, buf.data(), buf.size(), HAL_MAX_DELAY);
    }

    template<>
    std::expected<std::array<std::bitset<8>, 3>, HAL_StatusTypeDef> Transceiver::read() const {
        std::array<uint8_t, 3> buf;
        const auto err_ret { HAL_I2C_Mem_Read(hi2c, slave_address, static_cast<uint16_t>(Commands::Status::READ.to_ulong()), 2, buf.data(), buf.size(), HAL_MAX_DELAY) };
        if(err_ret != HAL_OK) {
            return std::unexpected(err_ret);
        }

        std::array<std::bitset<8>, buf.size()> ret;
        for(size_t i = 0; i < ret.size(); i++) {
            ret[i] = std::bitset<8>(buf[i]);
        }

        return ret;
    }

    template<>
    std::expected<std::array<std::bitset<8>, 6>, HAL_StatusTypeDef> Transceiver::read() const {
        std::array<uint8_t, 6> buf;
        
        const auto err_ret { HAL_I2C_Master_Receive(hi2c, slave_address, buf.data(), buf.size(), HAL_MAX_DELAY) };
        if(err_ret != HAL_OK) {
            return std::unexpected(err_ret);
        }

        std::array<std::bitset<8>, buf.size()> ret;
        for(size_t i = 0; i < ret.size(); i++) {
            ret[i] = std::bitset<8>(buf[i]);
        }

        return ret;
    }

    std::bitset<8> Transceiver::calculate_crc(const std::array<std::bitset<8>, 2>& data) {
        std::bitset<8> ret { 0xFF }; // initialization
        for(const auto& byte : data) {
            ret ^= byte;
            for(uint8_t bit = 0; bit < 8; bit++) {
                if(ret[7]) {
                    ret <<= 1;
                    ret ^= 0x31; // polynomial
                } else {
                    ret <<= 1;
                }
            }
        }

        return ret ^ std::bitset<8>(0x00); // final XOR
    }
}
}