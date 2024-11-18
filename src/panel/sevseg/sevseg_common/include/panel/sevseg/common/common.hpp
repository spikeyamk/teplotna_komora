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
        0b0111'0111, // 65  'A'
        0b0001'1111, // 66  'b'
        0b0100'1110, // 67  'C'
        0b0011'1101, // 68  'd'
        0b0100'1111, // 69  'E'
        0b0100'0111, // 70  'F'
        0b0101'1110, // 71  'G'
        0b0001'0111, // 72  'h'
        0b0000'0110, // 73  'i'
        0b0011'1000, // 74  'J'
        0b0011'0111, // 75  'K'
        0b0000'1110, // 76  'L'
        0b0101'0101, // 77  'm'
        0b0001'0101, // 78  'n'
        0b0001'1101, // 79  'O'
        0b0110'0111, // 80  'P'
        0b0111'0011, // 81  'q'
        0b0000'0101, // 82  'r'
        0b0101'1011, // 83  'S'
        0b0000'1111, // 84  't'
        0b0011'1110, // 85  'U'
        0b0010'1010, // 86  'V'
        0b0010'1011, // 87  'W'
        0b0001'0100, // 88  'x'
        0b0011'1011, // 89  'y'
        0b0110'1101, // 90  'Z'  Same as '2'
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