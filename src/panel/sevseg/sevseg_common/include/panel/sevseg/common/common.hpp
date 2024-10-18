#pragma once

#include <array>
#include <bitset>

namespace panel {
namespace sevseg {
namespace common {
    using sevset = std::bitset<8>;
    extern const std::array<sevset, 16> hex_map;
    extern const sevset minus_sign;
    extern const sevset dp_or_mask;
    using sevmap = std::array<sevset, 5>;
    extern const sevmap overflow_sevmap;
    extern const sevmap underflow_sevmap;

    enum class DP_Error {
        invalid_input,
        overflow
    };

    sevmap float_to_sevmap(const float value);
}
}
}