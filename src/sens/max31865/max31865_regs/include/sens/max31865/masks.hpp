#pragma once

#include <bitset>

namespace sens {
namespace max31865 {
    struct Masks {
        using Mask = std::bitset<8>;
        struct Configuration {
            struct Vbias {
                enum class Or {
                    OFF = 0b0000'0000,
                    ON  = 0b1000'0000,
                };

                static constexpr Mask AND { 0b1000'0000 };
            };

            struct ConversionMode {
                enum class Or {
                    OFF      = 0b0000'0000,
                    ONE_SHOT = 0b0010'0000,
                    AUTO     = 0b0100'0000,
                };

                static constexpr Mask AND { 0b0110'0000 };
            };

            struct WireMode {
                enum class Or {
                    TWO_WIRE_OR_FOUR_WIRE = 0b0000'0000,
                    THREE_WIRE            = 0b0001'0000,
                };

                static constexpr Mask AND { 0b0001'0000 };
            };

            struct FaultDetection {
                struct WriteAction {
                    enum class Or {
                        NO_ACTION                                          = 0b0000'0000,
                        FAULT_DETECTION_WITH_AUTOMATIC_DELAY               = 0b0000'0100,
                        RUN_FAULT_DETECTION_WITH_MANUAL_DELAY_CYCLE_ONE    = 0b0000'1000,
                        FINISH_FAULT_DETECTION_WITH_MANUAL_DELAY_CYCLE_TWO = 0b0000'1100,
                    };

                    static constexpr Mask AND { 0b0000'1100 };
                };

                struct ReadMeaning {
                    enum class Or {
                        FAULT_DETECTION_FINISHED                                        = 0b0000'0000,
                        AUTOMATIC_FAULT_DETECTION_STILL_RUNNING                         = 0b0000'0100,
                        MANUAL_CYCLE_ONE_STILL_RUNNING_WAITING_FOR_USER_TO_WRITE_ELEVEN = 0b0000'1000,
                        MANUAL_CYCLE_TWO_STILL_RUNNING                                  = 0b0000'1100,
                    };

                    static constexpr Mask AND { 0b0000'1100 };
                };

            };

            struct FaultStatusAutoClear {
                enum class Or {
                    NOCLEAR = 0b0000'0000,
                    CLEAR   = 0b0000'0010,
                };

                static constexpr Mask AND { 0b0000'0010 };
            };

            struct FilterSelect {
                enum class Or {
                    SIXTY_HZ = 0b0000'0000,
                    FIFTY_HZ = 0b0000'0001,
                };

                static constexpr Mask AND { 0b0000'0001 };
            };
        };

        struct RTD_LSBs {
            struct Fault {
                enum class Or {
                    NOFAULT = 0b0000'0000,
                    FAULT   = 0b0000'0001,
                };

                static constexpr Mask AND { 0b0000'0001 };
            };

            struct Data {
                static constexpr Mask AND { 0b1111'1110 };
            };
        };

        struct FaultStatus {
            struct AlwaysActive {
                enum class Or {
                    NOFAULT                     = 0b0000'0000,
                    OVERVOLTAGE_OR_UNDERVOLTAGE = 0b0000'0010,
                };

                static constexpr Mask AND { 0b0000'0010 };
            };

            struct MasterInitiated {
                struct RTDIN_MinusIsLowerThanZeroPointEightFiveTimesVbiasForceMinusOpen {
                    enum class Or {
                        NOFAULT = 0b0000'0000,
                        FAULT   = 0b0000'1000,
                    };

                    static constexpr Mask AND { 0b0000'1000 };
                };

                struct REFIN_MinusIsLowerThanZeroPointEightFiveTimesVbiasForceMinusOpen {
                    enum class Or {
                        NOFAULT = 0b0000'0000,
                        FAULT   = 0b0001'0000,
                    };

                    static constexpr Mask AND { 0b0001'0000 };
                };

                struct REFIN_MinusIsGreaterThanZeroPointEightFiveTimesVbias {
                    enum class Or {
                        NOFAULT = 0b0000'0000,
                        FAULT   = 0b0010'0000,
                    };

                    static constexpr Mask AND { 0b0010'0000 };
                };
            };

            struct EveryConversion {
                enum class Or {
                    NOFAULT     = 0b0000'0000,
                    RTD_SHORTED = 0b0100'0000,
                    RTD_OPEN    = 0b1000'0000,
                };

                static constexpr Mask AND { 0b1100'0000 };
            };
        };
    };
}
}