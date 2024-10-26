#pragma once

#include <cstdint>
#include <expected>
#include <bitset>
#include <array>

#include "stm32f2xx_hal.h"

#include "sens/max31865/reg_addrs.hpp"

namespace sens {
namespace max31865 {
    class Transceiver {
    private:
        bool inited { false };
        class Selector {
            GPIO_TypeDef* nss_port;
            const uint16_t nss_pin;
        public:
            Selector() = delete;
            inline Selector(GPIO_TypeDef* nss_port, const uint16_t nss_pin) :
                nss_port { nss_port },
                nss_pin { nss_pin }
            {
                HAL_GPIO_WritePin(nss_port, nss_pin, GPIO_PIN_RESET);
            }

            inline ~Selector() {
                HAL_GPIO_WritePin(nss_port, nss_pin, GPIO_PIN_SET);
            }
        };
    public:
        SPI_HandleTypeDef* hspi;
        GPIO_TypeDef* nss_port;
        const uint16_t nss_pin;
    public:
        Transceiver() = delete;
        Transceiver(SPI_HandleTypeDef* hspi, GPIO_TypeDef* nss_port, const uint16_t nss_pin) :
            hspi { hspi },
            nss_port { nss_port },
            nss_pin { nss_pin }
        {}
    public:
        HAL_StatusTypeDef init();
        bool is_inited() const;
        HAL_StatusTypeDef write(const RegAddrs::RW address, const std::bitset<8>& value) const;
        std::expected<std::bitset<8>, HAL_StatusTypeDef> read(const RegAddrs::RO address) const;
        std::expected<std::array<std::bitset<8>, 8>, HAL_StatusTypeDef> read_all() const;
    };
}
}