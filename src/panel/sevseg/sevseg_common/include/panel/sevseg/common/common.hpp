#pragma once

#include <array>
#include <bitset>

namespace panel {
namespace sevseg {
namespace common {
    using sevset = std::bitset<8>;
    using sevmap = std::array<sevset, 5>;
    extern const std::array<sevset, 16> hex_map;
    extern const std::array<sevset, 8> single_segment_map;
    sevmap float_to_sevmap(const float value);
}
}
}