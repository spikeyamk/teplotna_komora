#pragma once

#include <cstdint>
#include <bitint.hpp>

#include "stm32f2xx_hal.h"

#include "util/tmp.hpp"

namespace bksram {
    using uint20_t = bitint::ubitint<20>;

    struct ErrorCodes {
        static constexpr uint20_t CLEAR      { 0xF'FF'FF };
        static constexpr uint20_t MISSING_LF { 0xE'13'37 };
        struct TWDG {
            static constexpr uint20_t INIT   { 0xE'1A'35 };
            static constexpr uint20_t EXPIRE { 0xE'0A'35 };
        };

        struct RS232_UART {
            static constexpr uint20_t LAUNCH      { 0xE'23'00 };
            static constexpr uint20_t SEMAPHORE_NULLPTR { 0xE'23'01 };
        };

        struct TempCtl {
            static constexpr uint20_t LAUNCH { 0xE'7E'00 };
        };

        struct Panel {
            static constexpr uint20_t LAUNCH { 0xE'5E'80 };
        };

        struct SevsegWhite {
            static constexpr uint20_t LAUNCH { 0xE'55'50 };
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

        struct SenserKiller {
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

                        struct RTD {
                            struct NoSem {
                                static constexpr uint20_t FRONT { 0xE'31'9A };
                                static constexpr uint20_t REAR  { 0xE'31'9B };
                            };

                            struct NoSemFault {
                                static constexpr uint20_t FRONT { 0xE'31'9C };
                                static constexpr uint20_t REAR  { 0xE'31'9D };
                            };

                            struct Sem {
                                static constexpr uint20_t FRONT { 0xE'31'9E };
                                static constexpr uint20_t REAR  { 0xE'31'9F };
                            };

                            struct SemFault {
                                static constexpr uint20_t FRONT { 0xE'31'A0 };
                                static constexpr uint20_t REAR  { 0xE'31'A1 };
                            };
                        };
                    };
                };

                struct SHT31 {
                    struct Init {
                        struct HAL {
                            static constexpr uint20_t INSIDE { 0xE'31'A2 };
                            static constexpr uint20_t OUTSIDE { 0xE'31'A3 };
                        };

                        struct Crc {
                            static constexpr uint20_t INSIDE { 0xE'31'A4 };
                            static constexpr uint20_t OUTSIDE { 0xE'31'A5 };
                        };

                        struct DisableHeater {
                            static constexpr uint20_t INSIDE { 0xE'31'A6 };
                            static constexpr uint20_t OUTSIDE { 0xE'31'A7 };
                        };

                        struct ReadStatus {
                            static constexpr uint20_t INSIDE { 0xE'31'A8 };
                            static constexpr uint20_t OUTSIDE { 0xE'31'A9 };
                        };

                        struct StatusHeater {
                            static constexpr uint20_t INSIDE { 0xE'31'AA };
                            static constexpr uint20_t OUTSIDE { 0xE'31'AB };
                        };

                        struct Start {
                            static constexpr uint20_t INSIDE { 0xE'31'AC };
                            static constexpr uint20_t OUTSIDE { 0xE'31'AD };
                        };
                    };

                    struct TempHum {
                        static constexpr uint20_t INSIDE { 0xE'31'B0 };
                        static constexpr uint20_t OUTSIDE { 0xE'31'B1 };
                    };
                };
            };

            struct Worker {
                static constexpr uint20_t INITED_FALSE { 0xE'31'1F };

                struct MAX31865 {
                    struct RTD {
                        struct Timeout {
                            static constexpr uint20_t FRONT { 0xE'31'F0 };
                            static constexpr uint20_t REAR  { 0xE'31'F1 };
                        };

                        struct HighOrLowFaultThreshold {
                            static constexpr uint20_t FRONT { 0xE'31'F2 };
                            static constexpr uint20_t REAR  { 0xE'31'F3 };
                        };
                    };

                    struct RunAutoFaultDetection {
                        static constexpr uint20_t FRONT { 0xE'31'F4 };
                        static constexpr uint20_t REAR  { 0xE'31'F5 };
                    };
                };

                struct SHT31 {
                    struct TempHum {
                        static constexpr uint20_t INSIDE { 0xE'31'F6 };
                        static constexpr uint20_t OUTSIDE { 0xE'31'F7 };
                    };
                };
            };
        };
        
        using Registry = util::Registry<uint20_t,
            CLEAR,
            MISSING_LF,
            TWDG::INIT,
            TWDG::EXPIRE,

            RS232_UART::LAUNCH,
            RS232_UART::SEMAPHORE_NULLPTR,

            TempCtl::LAUNCH,
            
            Panel::LAUNCH,
            SevsegWhite::LAUNCH,

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

            SenserKiller::LAUNCH,

            SenserKiller::Init::MAX31865::TransceiverInit::FRONT,
            SenserKiller::Init::MAX31865::TransceiverInit::REAR,

            SenserKiller::Init::MAX31865::Extension::Init::FRONT,
            SenserKiller::Init::MAX31865::Extension::Init::REAR,
            
            SenserKiller::Init::MAX31865::Extension::ClearConfigure::FRONT,
            SenserKiller::Init::MAX31865::Extension::ClearConfigure::REAR,
            
            SenserKiller::Init::MAX31865::Extension::ClearConfigureValidation::FRONT,
            SenserKiller::Init::MAX31865::Extension::ClearConfigureValidation::REAR,
            
            SenserKiller::Init::MAX31865::Extension::SetFilterSelect::FRONT,
            SenserKiller::Init::MAX31865::Extension::SetFilterSelect::REAR,
            
            SenserKiller::Init::MAX31865::Extension::FilterSelectValidation::FRONT,
            SenserKiller::Init::MAX31865::Extension::FilterSelectValidation::REAR,
            
            SenserKiller::Init::MAX31865::Extension::SetFaultThreshold::FRONT,
            SenserKiller::Init::MAX31865::Extension::SetFaultThreshold::REAR,
            
            SenserKiller::Init::MAX31865::Extension::FaultThresholdValidation::FRONT,
            SenserKiller::Init::MAX31865::Extension::FaultThresholdValidation::REAR,
            
            SenserKiller::Init::MAX31865::Extension::Configure::FRONT,
            SenserKiller::Init::MAX31865::Extension::Configure::REAR,
            
            SenserKiller::Init::MAX31865::Extension::ConfigureValidation::FRONT,
            SenserKiller::Init::MAX31865::Extension::ConfigureValidation::REAR,
            
            SenserKiller::Init::MAX31865::Extension::ClearFaultStatus::FRONT,
            SenserKiller::Init::MAX31865::Extension::ClearFaultStatus::REAR,
            
            SenserKiller::Init::MAX31865::Extension::ClearFaultStatusValidation::FRONT,
            SenserKiller::Init::MAX31865::Extension::ClearFaultStatusValidation::REAR,
            
            SenserKiller::Init::MAX31865::Extension::RunAutoFaultDetection::FRONT,
            SenserKiller::Init::MAX31865::Extension::RunAutoFaultDetection::REAR,
            
            SenserKiller::Init::MAX31865::Extension::RTD::NoSem::FRONT,
            SenserKiller::Init::MAX31865::Extension::RTD::NoSem::REAR,

            SenserKiller::Init::MAX31865::Extension::RTD::NoSemFault::FRONT,
            SenserKiller::Init::MAX31865::Extension::RTD::NoSemFault::REAR,

            SenserKiller::Init::MAX31865::Extension::RTD::Sem::FRONT,
            SenserKiller::Init::MAX31865::Extension::RTD::Sem::REAR,

            SenserKiller::Init::MAX31865::Extension::RTD::SemFault::FRONT,
            SenserKiller::Init::MAX31865::Extension::RTD::SemFault::REAR,

            SenserKiller::Init::SHT31::Init::HAL::INSIDE,
            SenserKiller::Init::SHT31::Init::HAL::OUTSIDE,

            SenserKiller::Init::SHT31::Init::Crc::INSIDE,
            SenserKiller::Init::SHT31::Init::Crc::OUTSIDE,

            SenserKiller::Init::SHT31::Init::DisableHeater::INSIDE,
            SenserKiller::Init::SHT31::Init::DisableHeater::OUTSIDE,

            SenserKiller::Init::SHT31::Init::ReadStatus::INSIDE,
            SenserKiller::Init::SHT31::Init::ReadStatus::OUTSIDE,

            SenserKiller::Init::SHT31::Init::StatusHeater::INSIDE,
            SenserKiller::Init::SHT31::Init::StatusHeater::OUTSIDE,

            SenserKiller::Init::SHT31::Init::Start::INSIDE,
            SenserKiller::Init::SHT31::Init::Start::OUTSIDE,

            SenserKiller::Init::SHT31::TempHum::INSIDE,
            SenserKiller::Init::SHT31::TempHum::OUTSIDE,

            SenserKiller::Worker::INITED_FALSE,

            SenserKiller::Worker::MAX31865::RTD::Timeout::FRONT,
            SenserKiller::Worker::MAX31865::RTD::Timeout::REAR,

            SenserKiller::Worker::MAX31865::RTD::HighOrLowFaultThreshold::FRONT,
            SenserKiller::Worker::MAX31865::RTD::HighOrLowFaultThreshold::REAR,

            SenserKiller::Worker::MAX31865::RunAutoFaultDetection::FRONT,
            SenserKiller::Worker::MAX31865::RunAutoFaultDetection::REAR,

            SenserKiller::Worker::SHT31::TempHum::INSIDE,
            SenserKiller::Worker::SHT31::TempHum::OUTSIDE
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

    inline uint20_t read() {
        HAL_PWR_EnableBkUpAccess();
        HAL_PWREx_EnableBkUpReg();
        __HAL_RCC_BKPSRAM_CLK_ENABLE();

        return *reinterpret_cast<uint32_t*>(BKPSRAM_BASE);
    }

    bool test();
}