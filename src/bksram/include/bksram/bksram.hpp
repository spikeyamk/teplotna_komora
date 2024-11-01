#pragma once

#include <cstdint>
#include <ubitint.hpp>
#include "stm32f2xx_hal.h"

#include "util/util.hpp"

namespace bksram {
    using uint20_t = ubitint_t<20>;

    struct ErrorCodes {
        static constexpr uint20_t MISSING_LF { 0x0'13'37 };
        static constexpr uint20_t TWDG       { 0x0'0A'35 };

        struct Init {
            struct Fan {
                static constexpr uint20_t CTL_ALL_INIT { 0xE'FA'90 };
                static constexpr uint20_t FB_ALL_INIT  { 0xE'FA'91 };
                static constexpr uint20_t FB_ALL_TEST  { 0xE'FA'92 };
            };

            struct MAX31865 {
                struct TransceiverInit {
                    static constexpr uint20_t FRONT { 0xE'31'70 };
                    static constexpr uint20_t REAR  { 0xE'31'71 };
                };

                struct Extension {
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

        struct Running {
            static constexpr uint20_t FAN_FB { 0xE'FA'A0 };

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

        using Registry = util::Registry<uint20_t,
            MISSING_LF,
            TWDG,

            Init::Fan::CTL_ALL_INIT,
            Init::Fan::FB_ALL_INIT,
            Init::Fan::FB_ALL_TEST,
            
            Init::MAX31865::TransceiverInit::FRONT,
            Init::MAX31865::TransceiverInit::REAR,
            
            Init::MAX31865::Extension::ClearConfigure::FRONT,
            Init::MAX31865::Extension::ClearConfigure::REAR,
            
            Init::MAX31865::Extension::ClearConfigureValidation::FRONT,
            Init::MAX31865::Extension::ClearConfigureValidation::REAR,
            
            Init::MAX31865::Extension::SetFilterSelect::FRONT,
            Init::MAX31865::Extension::SetFilterSelect::REAR,
            
            Init::MAX31865::Extension::FilterSelectValidation::FRONT,
            Init::MAX31865::Extension::FilterSelectValidation::REAR,
            
            Init::MAX31865::Extension::SetFaultThreshold::FRONT,
            Init::MAX31865::Extension::SetFaultThreshold::REAR,
            
            Init::MAX31865::Extension::FaultThresholdValidation::FRONT,
            Init::MAX31865::Extension::FaultThresholdValidation::REAR,
            
            Init::MAX31865::Extension::Configure::FRONT,
            Init::MAX31865::Extension::Configure::REAR,
            
            Init::MAX31865::Extension::ConfigureValidation::FRONT,
            Init::MAX31865::Extension::ConfigureValidation::REAR,
            
            Init::MAX31865::Extension::ClearFaultStatus::FRONT,
            Init::MAX31865::Extension::ClearFaultStatus::REAR,
            
            Init::MAX31865::Extension::ClearFaultStatusValidation::FRONT,
            Init::MAX31865::Extension::ClearFaultStatusValidation::REAR,
            
            Init::MAX31865::Extension::RunAutoFaultDetection::FRONT,
            Init::MAX31865::Extension::RunAutoFaultDetection::REAR,

            Running::FAN_FB,

            Running::MAX31865::RTD::Timeout::FRONT,
            Running::MAX31865::RTD::Timeout::REAR,

            Running::MAX31865::RTD::HighOrLowFaultThreshold::FRONT,
            Running::MAX31865::RTD::HighOrLowFaultThreshold::REAR,

            Running::MAX31865::RunAutoFaultDetection::FRONT,
            Running::MAX31865::RunAutoFaultDetection::REAR
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