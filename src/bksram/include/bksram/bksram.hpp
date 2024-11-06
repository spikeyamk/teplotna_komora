#pragma once

#include <cstdint>
#include <ubitint.hpp>
#include "stm32f2xx_hal.h"

#include "util/util.hpp"

namespace bksram {
    using uint20_t = ubitint_t<20>;

    struct ErrorCodes {
        static constexpr uint20_t MISSING_LF { 0xE'13'37 };
        static constexpr uint20_t TWDG       { 0xE'0A'35 };

        struct RS232_UART {
            static constexpr uint20_t LAUNCH { 0xE'23'00 };
        };

        struct TempCtl {
            static constexpr uint20_t LAUNCH { 0xE'7E'00 };
        };

        struct Panel {
            static constexpr uint20_t LAUNCH { 0xE'5E'80 };
        };

        struct FanSenser {
            static constexpr uint20_t LAUNCH { 0xE'F5'00 };

            struct Init {
                static constexpr uint20_t FB_ALL  { 0xE'F5'1A };

                static constexpr uint20_t FAN0_RL { 0xE'F5'10 };
                static constexpr uint20_t FAN1_FL { 0xE'F5'11 };
                static constexpr uint20_t FAN2_RR { 0xE'F5'12 };
                static constexpr uint20_t FAN3_BL { 0xE'F5'13 };
                static constexpr uint20_t FAN4_BR { 0xE'F5'14 };
                static constexpr uint20_t FAN5_FR { 0xE'F5'15 };
                using FanRegistry = util::Registry<uint20_t,
                    FAN0_RL,
                    FAN1_FL,
                    FAN2_RR,
                    FAN3_BL,
                    FAN4_BR,
                    FAN5_FR
                >;
            };

            struct Worker {
                static constexpr uint20_t INITED_FALSE { 0xE'F5'1F };

                static constexpr uint20_t FAN0_RL { 0xE'F5'A0 };
                static constexpr uint20_t FAN1_FL { 0xE'F5'A1 };
                static constexpr uint20_t FAN2_RR { 0xE'F5'A2 };
                static constexpr uint20_t FAN3_BL { 0xE'F5'A3 };
                static constexpr uint20_t FAN4_BR { 0xE'F5'A4 };
                static constexpr uint20_t FAN5_FR { 0xE'F5'A5 };
                using FanRegistry = util::Registry<uint20_t,
                    FAN0_RL,
                    FAN1_FL,
                    FAN2_RR,
                    FAN3_BL,
                    FAN4_BR,
                    FAN5_FR
                >;
            };
        };

        struct TempSenser {
            static constexpr uint20_t LAUNCH { 0xE'31'00 };
            struct Init {
                struct MAX31865 {
                    struct TransceiverInit {
                        static constexpr uint20_t FRONT { 0xE'31'70 };
                        static constexpr uint20_t REAR  { 0xE'31'71 };
                    };

                    struct Extension {
                        struct Init {
                            static constexpr uint20_t FRONT { 0xE'31'80 };
                            static constexpr uint20_t REAR  { 0xE'31'81 };
                        };

                        struct ClearConfigure {
                            static constexpr uint20_t FRONT { 0xE'31'82 };
                            static constexpr uint20_t REAR  { 0xE'31'83 };
                        };

                        struct ClearConfigureValidation {
                            static constexpr uint20_t FRONT { 0xE'31'84 };
                            static constexpr uint20_t REAR  { 0xE'31'85 };
                        };

                        struct SetFilterSelect {
                            static constexpr uint20_t FRONT { 0xE'31'86 };
                            static constexpr uint20_t REAR  { 0xE'31'87 };
                        };

                        struct FilterSelectValidation {
                            static constexpr uint20_t FRONT { 0xE'31'88 };
                            static constexpr uint20_t REAR  { 0xE'31'89 };
                        };

                        struct SetFaultThreshold {
                            static constexpr uint20_t FRONT { 0xE'31'8A };
                            static constexpr uint20_t REAR  { 0xE'31'8B };
                        };

                        struct FaultThresholdValidation {
                            static constexpr uint20_t FRONT { 0xE'31'8C };
                            static constexpr uint20_t REAR  { 0xE'31'8D };
                        };

                        struct Configure {
                            static constexpr uint20_t FRONT { 0xE'31'90 };
                            static constexpr uint20_t REAR  { 0xE'31'91 };
                        };

                        struct ConfigureValidation {
                            static constexpr uint20_t FRONT { 0xE'31'92 };
                            static constexpr uint20_t REAR  { 0xE'31'93 };
                        };

                        struct ClearFaultStatus {
                            static constexpr uint20_t FRONT { 0xE'31'94 };
                            static constexpr uint20_t REAR  { 0xE'31'95 };
                        };

                        struct ClearFaultStatusValidation {
                            static constexpr uint20_t FRONT { 0xE'31'96 };
                            static constexpr uint20_t REAR  { 0xE'31'97 };
                        };

                        struct RunAutoFaultDetection {
                            static constexpr uint20_t FRONT { 0xE'31'98 };
                            static constexpr uint20_t REAR  { 0xE'31'99 };
                        };
                    };
                };
            };

            struct Worker {
                static constexpr uint20_t INITED_FALSE { 0xE'31'1F };

                struct MAX31865 {
                    struct RTD {
                        struct Timeout {
                            static constexpr uint20_t FRONT { 0xE'31'A0 };
                            static constexpr uint20_t REAR  { 0xE'31'A1 };
                        };

                        struct HighOrLowFaultThreshold {
                            static constexpr uint20_t FRONT { 0xE'31'A2 };
                            static constexpr uint20_t REAR  { 0xE'31'A3 };
                        };
                    };

                    struct RunAutoFaultDetection {
                        static constexpr uint20_t FRONT { 0xE'31'B0 };
                        static constexpr uint20_t REAR  { 0xE'31'B1 };
                    };
                };
            };
        };
        
        using Registry = util::Registry<uint20_t,
            MISSING_LF,
            TWDG,

            RS232_UART::LAUNCH,

            TempCtl::LAUNCH,
            
            Panel::LAUNCH,

            FanSenser::LAUNCH,
            FanSenser::Init::FB_ALL,

            FanSenser::Init::FAN0_RL,
            FanSenser::Init::FAN1_FL,
            FanSenser::Init::FAN2_RR,
            FanSenser::Init::FAN3_BL,
            FanSenser::Init::FAN4_BR,
            FanSenser::Init::FAN5_FR,

            FanSenser::Worker::INITED_FALSE,
            FanSenser::Worker::FAN0_RL,
            FanSenser::Worker::FAN1_FL,
            FanSenser::Worker::FAN2_RR,
            FanSenser::Worker::FAN3_BL,
            FanSenser::Worker::FAN4_BR,
            FanSenser::Worker::FAN5_FR,

            TempSenser::LAUNCH,

            TempSenser::Init::MAX31865::TransceiverInit::FRONT,
            TempSenser::Init::MAX31865::TransceiverInit::REAR,

            TempSenser::Init::MAX31865::Extension::Init::FRONT,
            TempSenser::Init::MAX31865::Extension::Init::REAR,
            
            TempSenser::Init::MAX31865::Extension::ClearConfigure::FRONT,
            TempSenser::Init::MAX31865::Extension::ClearConfigure::REAR,
            
            TempSenser::Init::MAX31865::Extension::ClearConfigureValidation::FRONT,
            TempSenser::Init::MAX31865::Extension::ClearConfigureValidation::REAR,
            
            TempSenser::Init::MAX31865::Extension::SetFilterSelect::FRONT,
            TempSenser::Init::MAX31865::Extension::SetFilterSelect::REAR,
            
            TempSenser::Init::MAX31865::Extension::FilterSelectValidation::FRONT,
            TempSenser::Init::MAX31865::Extension::FilterSelectValidation::REAR,
            
            TempSenser::Init::MAX31865::Extension::SetFaultThreshold::FRONT,
            TempSenser::Init::MAX31865::Extension::SetFaultThreshold::REAR,
            
            TempSenser::Init::MAX31865::Extension::FaultThresholdValidation::FRONT,
            TempSenser::Init::MAX31865::Extension::FaultThresholdValidation::REAR,
            
            TempSenser::Init::MAX31865::Extension::Configure::FRONT,
            TempSenser::Init::MAX31865::Extension::Configure::REAR,
            
            TempSenser::Init::MAX31865::Extension::ConfigureValidation::FRONT,
            TempSenser::Init::MAX31865::Extension::ConfigureValidation::REAR,
            
            TempSenser::Init::MAX31865::Extension::ClearFaultStatus::FRONT,
            TempSenser::Init::MAX31865::Extension::ClearFaultStatus::REAR,
            
            TempSenser::Init::MAX31865::Extension::ClearFaultStatusValidation::FRONT,
            TempSenser::Init::MAX31865::Extension::ClearFaultStatusValidation::REAR,
            
            TempSenser::Init::MAX31865::Extension::RunAutoFaultDetection::FRONT,
            TempSenser::Init::MAX31865::Extension::RunAutoFaultDetection::REAR,
            
            TempSenser::Worker::INITED_FALSE,

            TempSenser::Worker::MAX31865::RTD::Timeout::FRONT,
            TempSenser::Worker::MAX31865::RTD::Timeout::REAR,

            TempSenser::Worker::MAX31865::RTD::HighOrLowFaultThreshold::FRONT,
            TempSenser::Worker::MAX31865::RTD::HighOrLowFaultThreshold::REAR,

            TempSenser::Worker::MAX31865::RunAutoFaultDetection::FRONT,
            TempSenser::Worker::MAX31865::RunAutoFaultDetection::REAR
        >;
    };

    template<uint20_t value>
    inline void write() {
        HAL_PWR_EnableBkUpAccess();
        HAL_PWREx_EnableBkUpReg();
        __HAL_RCC_BKPSRAM_CLK_ENABLE();

        *reinterpret_cast<uint32_t*>(BKPSRAM_BASE) = ErrorCodes::Registry::get<value>().unwrap();
    }

    template<uint20_t value>
    inline void write_reset() {
        write<value>();
        NVIC_SystemReset();
    }

    inline ubitint_t<20> read() {
        HAL_PWR_EnableBkUpAccess();
        HAL_PWREx_EnableBkUpReg();
        __HAL_RCC_BKPSRAM_CLK_ENABLE();

        return *reinterpret_cast<uint32_t*>(BKPSRAM_BASE);
    }

    bool test();
}