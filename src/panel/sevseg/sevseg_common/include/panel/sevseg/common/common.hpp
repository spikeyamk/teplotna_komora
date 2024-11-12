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
        0b0111'0111, // A
        0b0001'1111, // b
        0b0100'1110, // C
        0b0011'1101, // d
        0b0100'1111, // E
        0b0100'0111, // F
        0b0101'1110, // G
        0b0001'0011, // h
        0b0000'0010, // I
        0b0011'1001, // J
        0b0100'1010, // K
        0b0000'1110, // L
        0b0011'0111, // m
        0b0011'0101, // n
        0b0101'1110, // o
        0b0111'0110, // P
        0b0011'1100, // Q
        0b0101'0100, // r
        0b0101'1011, // S
        0b0000'1111, // t
        0b0011'0010, // U
        0b0110'1110, // V
        0b0011'1011, // X
        0b0011'1011, // y
        0b0101'1100, // z
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