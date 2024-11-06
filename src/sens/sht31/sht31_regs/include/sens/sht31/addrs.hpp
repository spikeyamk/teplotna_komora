#pragma once

#include "sens/sht31/masks.hpp"

namespace sens {
namespace sht31 {
    enum class SlaveAddress {
        ADDR_PIN_LOW = 0x44,
        ADDR_PIN_HIGH = 0x45,
    };

    struct Commands {
        struct SingleShot {
            enum class ClockStretchingEnabled {
                HIGH_REPEATABILITY = 0x2C'06,
                MEDIUM_REPEATABILITY = 0x2C'0D,
                LOW_REPEATABILITY = 0x2C'10,
            };

            enum class ClockStretchingDisabled {
                HIGH_REPEATABILITY = 0x24'00,
                MEDIUM_REPEATABILITY = 0x24'0B,
                LOW_REPEATABILITY = 0x24'16,
            };
        };

        struct Periodic {
            enum class HALF_HERTZ {
                HIGH_REPEATABILITY = 0x20'32,
                MEDIUM_REPEATABILITY = 0x20'24,
                LOW_REPEATABILITY = 0x20'2F,
            };

            enum class ONE_HERTZ {
                HIGH_REPEATABILITY = 0x21'30,
                MEDIUM_REPEATABILITY = 0x21'26,
                LOW_REPEATABILITY = 0x21'2D,
            };

            enum class TWO_HERTZ {
                HIGH_REPEATABILITY = 0x22'36,
                MEDIUM_REPEATABILITY = 0x22'20,
                LOW_REPEATABILITY = 0x22'2B,
            };

            enum class FOUR_HERTZ {
                HIGH_REPEATABILITY = 0x23'34,
                MEDIUM_REPEATABILITY = 0x23'22,
                LOW_REPEATABILITY = 0x23'29,
            };

            enum class TEN_HERTZ {
                HIGH_REPEATABILITY = 0x27'37,
                MEDIUM_REPEATABILITY = 0x27'21,
                LOW_REPEATABILITY = 0x27'2A,
            };

            static constexpr Command FETCH_DATA { 0xE0'00 };
            static constexpr Command ART { 0x2B'32 };
            static constexpr Command BREAK { 0x30'93 };
        };

        static constexpr Command SOFT_RESET { 0x30'A2 };

        enum class Heater {
            ENABLE = 0x30'6D,
            DISABLE = 0x30'66,
        };

        struct Status {
            static constexpr Command READ { 0xF3'2D };
            static constexpr Command CLEAR { 0x30'41 };
        };

        // UNIMPLEMENTED !!!
        static constexpr Command GENERAL_CALL { 0x00'06 };
    };
}
}