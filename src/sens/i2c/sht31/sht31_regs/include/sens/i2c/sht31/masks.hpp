#pragma once

#include <bitset>

namespace sens {
namespace i2c {
namespace sht31 {
    using Command = std::bitset<16>;

    struct Masks {
        struct Status {
            struct LastWriteDataChecksum {
                enum class Or {
                    CORRECT = 0b0000'0000'0000'0000,
                    FAILED  = 0b0000'0000'0000'0001,
                };

                static constexpr Command AND { 0b0000'0000'0000'0001 };
            };

            struct LastCommand {
                enum class Or {
                    EXECUTED_SUCCESSFULLY      = 0b0000'0000'0000'0000,
                    INVALID_OR_FAILED_CHECKSUM = 0b0000'0000'0000'0010,
                };

                static constexpr Command AND { 0b0000'0000'0000'0010 };
            };

            struct SystemReset {
                enum class Or {
                    NOT_DETECTED = 0b0000'0000'0000'0000,
                    DETECTED     = 0b0000'0000'0001'0000,
                };

                static constexpr Command AND { 0b0000'0000'0001'0000 };
            };


            struct TrackingAlert {
                struct Temp {
                    enum class Or {
                        NOALERT = 0b0000'0000'0000'0000,
                        ALERT   = 0b0000'0100'0000'0000,
                    };

                    static constexpr Command AND { 0b0000'0100'0000'0000 };
                };

                struct Hum {
                    enum class Or {
                        NOALERT = 0b0000'0000'0000'0000,
                        ALERT   = 0b0000'1000'0000'0000,
                    };

                    static constexpr Command AND { 0b0000'1000'0000'0000 };
                };
            };

            struct Heater {
                enum class Or {
                    OFF = 0b0000'0000'0000'0000,
                    ON  = 0b0010'0000'0000'0000,
                };

                static constexpr Command AND { 0b0010'0000'0000'0000 };
            };

            struct AlertPending {
                enum class Or {
                    NO_PENDING           = 0b0000'0000'0000'0000,
                    AT_LEAST_ONE_PENDING = 0b1000'0000'0000'0000,
                };

                static constexpr Command AND { 0b1000'0000'0000'0000 };
            };
        };
    };
}
}
}