#pragma once

#include <array>
#include <bitset>
#include <algorithm>
#include <cstdint>
#include <ranges>
#include <bitint.hpp>

#include "util/tmp.hpp"

namespace panel {
namespace sevseg {
namespace common {
    using sevset = std::bitset<8>;
    using sevmap = std::array<sevset, 5>;
    using uint20_t = bitint::ubitint<20>;

    extern const std::array<sevset, 16> hex_map;
    extern const std::array<sevset, 8> single_segment_map;

    constexpr std::array<sevset, 26> latin_map {
        0b01110111, // 65  'A'
        0b01111100, // 66  'b'
        0b00111001, // 67  'C'
        0b01011110, // 68  'd'
        0b01111001, // 69  'E'
        0b01110001, // 70  'F'
        0b00111101, // 71  'G'
        0b01110110, // 72  'H'
        0b00110000, // 73  'I'
        0b00001110, // 74  'J'
        0b01110110, // 75  'K'  Same as 'H'
        0b00111000, // 76  'L'
        0b00000000, // 77  'M'  NO DISPLAY
        0b01010100, // 78  'n'
        0b00111111, // 79  'O'
        0b01110011, // 80  'P'
        0b01100111, // 81  'q'
        0b01010000, // 82  'r'
        0b01101101, // 83  'S'
        0b01111000, // 84  't'
        0b00111110, // 85  'U'
        0b00111110, // 86  'V'  Same as 'U'
        0b00000000, // 87  'W'  NO DISPLAY
        0b01110110, // 88  'X'  Same as 'H'
        0b01101110, // 89  'y'
        0b01011011, // 90  'Z'  Same as '2'
    };

    extern const sevset minus_sign;
    extern const sevset dp_or_mask;

    namespace exception_sevmap {
        extern const sevmap error;
        extern const sevmap positive_overflow;
        extern const sevmap negative_overflow;
        extern const sevmap positive_underflow;
        extern const sevmap negative_underflow;
    }

    template<typename T>
    sevmap to_sevmap(const T obj);

    template<util::TemplateStringNonNullTerminated string>
    requires(
        (sizeof(string.value) <= 5)
        && std::ranges::all_of(
            std::ranges::subrange(string.value, string.value + sizeof(string.value)),
            [](const char c) { return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); }
        )
    )
    consteval sevmap to_sevmap() {
        sevmap ret {};

        for(auto&& [ch, r] : std::ranges::views::zip(string.value, ret)) {
            r = latin_map[static_cast<size_t>(ch - ((ch >= 'a' && ch <= 'z') ? 'a' : 'A'))];
        }

        return ret;
    }
}
}
}