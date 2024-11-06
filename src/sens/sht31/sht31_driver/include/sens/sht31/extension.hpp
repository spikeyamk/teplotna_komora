#pragma once

#include "sens/sht31/transceiver.hpp"
#include "sens/sht31/temp_hum.hpp"
#include "sens/sht31/status.hpp"

namespace sens {
namespace sht31 {
    class Extension {
    private:
        Transceiver& transceiver;
    public:
        Extension() = delete;
        Extension(Transceiver& transceiver) :
            transceiver { transceiver }
        {}
    public:
        std::expected<bool, HAL_StatusTypeDef> init() const;
        HAL_StatusTypeDef clear_status() const;
        std::expected<Status, HAL_StatusTypeDef> read_status() const;
        HAL_StatusTypeDef enable_heater() const;
        HAL_StatusTypeDef disable_heater() const;
        HAL_StatusTypeDef soft_reset() const;
        HAL_StatusTypeDef start_periodic_mode_ten_hertz_high_repeatability() const;
        HAL_StatusTypeDef stop_periodic_mode() const;
        std::expected<TempHum, HAL_StatusTypeDef> read_temp_hum_periodic_mode() const;
    };
}
}