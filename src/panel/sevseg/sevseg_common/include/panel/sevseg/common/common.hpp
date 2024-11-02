#pragma once

#include <array>
#include <bitset>
#include <ubitint.hpp>

using uint20_t = ubitint_t<20>;

namespace panel {
namespace sevseg {
namespace common {
    using sevset = std::bitset<8>;
    using sevmap = std::array<sevset, 5>;
    extern const std::array<sevset, 16> hex_map;
    extern const std::array<sevset, 8> single_segment_map;

    extern const sevset minus_sign;
    extern const sevset dp_or_mask;

    namespace exception_sevmap {
        extern const sevmap error;
        extern const sevmap positive_overflow;
        extern const sevmap negative_overflow;
        extern const sevmap positive_underflow;
        extern const sevmap negative_underflow;
    }

    sevmap float_to_sevmap(const float value);
    sevmap uint20_t_to_sevmap(const uint20_t value);
}
}
}