#include <algorithm>

#include "sens/max31865/masks.hpp"
#include "sens/max31865/transceiver.hpp"

namespace sens {
namespace max31865 {
    bool Transceiver::is_inited() const {
        return inited;
    }

    HAL_StatusTypeDef Transceiver::init() {
        if(is_inited() == true) {
            return HAL_OK;
        }

        Selector selector { nss_port, nss_pin };

        HAL_Delay(10);
        const std::array<uint8_t, 2> init_configuration {
            static_cast<uint8_t>(RegAddrs::RW::CONFIGURATION),
            static_cast<uint8_t>(Masks::FaultStatusAutoClear::Or::CLEAR)
        };
        const HAL_StatusTypeDef ret {
            HAL_SPI_Transmit(
                hspi,
                init_configuration.data(),
                init_configuration.size(),
                500
            )
        };
        inited = true;

        return ret;
    }

    HAL_StatusTypeDef Transceiver::write(const RegAddrs::RW address, const std::bitset<8>& value) const {
        if(is_inited() == false) {
            return HAL_ERROR;
        }

        Selector selector { nss_port, nss_pin };

        const std::array<uint8_t, 2> buf {
            static_cast<uint8_t>(address),
            static_cast<uint8_t>(value.to_ulong())
        };
        const HAL_StatusTypeDef ret { HAL_SPI_Transmit(hspi, buf.data(), buf.size(), 500) };

        return ret;
    }
    
    std::expected<std::bitset<8>, HAL_StatusTypeDef> Transceiver::read(const RegAddrs::RO address) const {
        if(is_inited() == false) {
            return std::unexpected(HAL_ERROR);
        }

        Selector selector { nss_port, nss_pin };

        const uint8_t buf { static_cast<uint8_t>(address) }; 
        HAL_StatusTypeDef ret_err { HAL_SPI_Transmit(hspi, &buf, sizeof(buf), 500) };
        if(ret_err != HAL_OK) {
            return std::unexpected(ret_err);
        }

        uint8_t ret { 0x00 };
        ret_err = HAL_SPI_Receive(hspi, &ret, sizeof(ret), 500);

        if(ret_err != HAL_OK) {
            return std::unexpected(ret_err);
        }

        return std::bitset<8> { ret };
    }

    std::expected<std::array<std::bitset<8>, 8>, HAL_StatusTypeDef> Transceiver::read_all() const {
        if(is_inited() == false) {
            return std::unexpected(HAL_ERROR);
        }

        Selector selector { nss_port, nss_pin };

        const uint8_t buf { static_cast<uint8_t>(RegAddrs::RO::CONFIGURATION) };
        HAL_SPI_Transmit(hspi, &buf, sizeof(buf), 500);
        std::array<uint8_t, 8> ret_regs {};
        for(uint8_t& e: ret_regs) {
            const HAL_StatusTypeDef ret_err { HAL_SPI_Receive(hspi, &e, sizeof(e), 500) };
            if(ret_err != HAL_OK) {
                return std::unexpected(ret_err);
            }
        }

        std::array<std::bitset<8>, ret_regs.size()> ret;
        std::transform(
            ret_regs.cbegin(),
            ret_regs.cend(),
            ret.begin(),
            [](const uint8_t e) {
                return std::bitset<8>(e);
            }
        );

        return ret;
    }

    HAL_StatusTypeDef Transceiver::dump() {
        const auto ret_read_all { read_all() };
        if(ret_read_all.has_value() == false) {
            return ret_read_all.error(); 
        }

        for(size_t i = 0; i < ret_read_all.value().size(); i++) {
            std::printf(
                "sens::max31865::Transceiver::dump: this: %p, ret_read_all.value()[%zu]: 0x%02lX\n",
                reinterpret_cast<void*>(this),
                i,
                ret_read_all.value()[i].to_ulong()
            );
        }

        return HAL_OK;
    }
}
}