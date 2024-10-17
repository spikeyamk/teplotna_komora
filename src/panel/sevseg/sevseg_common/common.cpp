#include <optional>
#include <cmath>
#include "panel/sevseg/common/common.hpp"

namespace panel {
namespace sevseg {
namespace common {
    const std::array<sevset, 16> hex_map {
        0b0111'1110, // 0
        0b0011'0000, // 1
        0b0110'1101, // 2
        0b0111'1001, // 3
        0b0011'0011, // 4
        0b0101'1011, // 5
        0b0101'1111, // 6
        0b0111'0000, // 7
        0b0111'1111, // 8
        0b0111'1011, // 9
        0b0111'0111, // A
        0b0001'1111, // b
        0b0100'1110, // C
        0b0011'1101, // d
        0b0100'1111, // E
        0b0100'0111  // F
    };

    const sevset minus_sign { 0b0000'0001 };
    const sevset dp_mask { 0b1000'0000 };


    std::optional<uint8_t> get_dp_index(const float value) {
        if(value == 0.0f) {
            return 0;
        }

        const float log10_value { std::log10(std::fabs(value)) };

        if(log10_value < -3.0f || log10_value >= 4.0f) {
            return std::nullopt;
        }

        if(log10_value >= 1.0f && log10_value < 2.0f) {
            return 1;
        } else if (log10_value >= 2 && log10_value < 3) {
            return 2;
        } else if (log10_value >= 3 && log10_value < 4) {
            return 3;
        }

        return 0;
    }

    const sevmap overflow_sevmap {
        []() {
            sevmap ret;
            ret.fill(hex_map[0xf]);
            return ret;
        }()
    };

    const sevmap underflow_sevmap {
        []() {
            sevmap ret;
            ret.fill(hex_map[0x0]);
            return ret;
        }()
    };

    sevmap float_to_sevmap(const float value) {
        std::array<std::bitset<8>, 5> ret {};

        const auto dp_index { get_dp_index(value) };


        const uint8_t zeroth_digit { 0 };
        const uint8_t first_digit { 0 };
        const uint8_t second_digit { 0 };
        const uint8_t third_digit { 0 };

        if(value < 0.0f) {
            ret[0] = minus_sign;
            ret[1] = hex_map[zeroth_digit];
            ret[2] = hex_map[first_digit];
            ret[3] = hex_map[second_digit];
            ret[4] = hex_map[third_digit];
        } else {
            ret[0] = hex_map[zeroth_digit];
            ret[1] = hex_map[first_digit];
            ret[2] = hex_map[second_digit];
            ret[3] = hex_map[third_digit];
            const uint8_t fourth_digit { 0 };
            ret[3] = hex_map[fourth_digit];
        }

        return ret;
    }
}
}
}