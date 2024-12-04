#pragma once

#include <cassert>

#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "sens/max31865/configuration.hpp"
#include "sens/max31865/fault_threshold.hpp"
#include "sens/max31865/rtd.hpp"
#include "sens/max31865/fault_status.hpp"
#include "sens/max31865/transceiver.hpp"

namespace sens {
namespace max31865 {
    class Extension {
    private:
        GPIO_TypeDef* ndrdy_port;
        const uint16_t ndrdy_pin;
        StaticSemaphore_t sem_control_block {};
        osSemaphoreId_t sem { nullptr };
    public:
        static constexpr uint32_t sem_timeout { 2'000 };
        Transceiver& transceiver;
    public:
        Extension(GPIO_TypeDef* ndrdy_port, const uint16_t ndrdy_pin, Transceiver& transceiver) :
            ndrdy_port { ndrdy_port },
            ndrdy_pin { ndrdy_pin },
            transceiver { transceiver }
        {}
    public:
        template<size_t N>
        requires (N <= 16)
        void init(const char (&name)[N]) {
            const osSemaphoreAttr_t sem_attr {
                .name = name,
                .attr_bits = 0,
                .cb_mem = &sem_control_block,
                .cb_size = sizeof(sem_control_block),
            };
            
            sem = osSemaphoreNew(1, 0, &sem_attr);
            assert(sem != nullptr);
        }

        HAL_StatusTypeDef configure(const Configuration& configuration) const;
        std::expected<Configuration, HAL_StatusTypeDef> read_configuration() const;

        HAL_StatusTypeDef set_fault_threshold(const FaultThreshold& fault_threshold) const;
        std::expected<FaultThreshold, HAL_StatusTypeDef> read_fault_threshold() const;

        HAL_StatusTypeDef set_filter_select(const Masks::FilterSelect::Or filter_select) const;
        std::expected<Masks::FilterSelect::Or, HAL_StatusTypeDef> read_filter_select() const;

        std::expected<RTD, HAL_StatusTypeDef> read_rtd_no_sem();
        std::expected<RTD, HAL_StatusTypeDef> read_rtd();
        osStatus release_sem();

        std::expected<FaultStatus, HAL_StatusTypeDef> run_auto_fault_detection() const;
        std::expected<FaultStatus, HAL_StatusTypeDef> read_fault_status() const;
        HAL_StatusTypeDef clear_fault_status() const;
    };
}
}