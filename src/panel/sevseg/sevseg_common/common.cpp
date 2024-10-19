#include <optional>
#include <cmath>
#include <cstdint>
#include <algorithm>
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
        0b0100'0111, // F
    };

    const std::array<sevset, 8> single_segment_map {
        0b0000'0001, // stred
        0b0000'0010, // lavo hore
        0b0000'0100, // lavo dole
        0b0000'1000, // dole
        0b0001'0000, // vpravo dole
        0b0010'0000, // vpravo hore
        0b0100'0000, // hore
        0b1000'0000, // bodka
    };

    const sevset minus_sign { 0b0000'0001 };
    const sevset dp_or_mask { 0b1000'0000 };

    namespace exception_sevmaps {
        const sevmap error {
            []() {
                sevmap ret;
                ret.fill(hex_map[0xe]);
                return ret;
            }()
        };

        const sevmap zero {
            []() {
                sevmap ret;
                ret.fill(hex_map[0x0]);
                ret.front() |= minus_sign;
                return ret;
            }()
        };

        const sevmap positive_overflow {
            []() {
                sevmap ret;
                ret.fill(hex_map[0xF]);
                ret.back() |= dp_or_mask;
                return ret;
            }()
        };

        const sevmap negative_overflow {
            []() {
                sevmap ret { minus_sign };
                std::fill(ret.begin() + 1, ret.end(), hex_map[0xF]);
                ret.back() |= dp_or_mask;
                return ret;
            }()
        };


        const sevmap& positive_underflow { zero };
        
        const sevmap negative_underflow {
            []() {
                sevmap ret { minus_sign };
                std::fill(ret.begin() + 1, ret.end(), hex_map[0x0]);
                *(ret.begin() + 1) |= dp_or_mask;
                return ret;
            }()
        };
    }

    inline uint8_t numerical_char_to_uint8_t(const char value) {
        return static_cast<uint8_t>(value - '0');
    }

    inline bool check_snprintf_output(const std::string_view& buf) {
        for(size_t i = 0; i < 6; i++) {
            if(
                buf[i] != '-'
                || buf[i] != '.'
                || (!(buf[i] >= '0' && buf[i] <= '9'))
            ) {
                return false;
            }
        }
        
        return true;
    }

    sevmap float_to_sevmap(const float value) {
        if(std::isnormal(value) == false) {
            return exception_sevmaps::error;
        } else if(value == 0.0f) {
            return exception_sevmaps::zero;
        } else if(value > 9999.9f) {
            return exception_sevmaps::positive_overflow;
        } else if(value < -999.9f) {
            return exception_sevmaps::negative_overflow;
        } else if(value < 0.0001) {
            return exception_sevmaps::positive_underflow;
        } else if(value > -0.001) {
            return exception_sevmaps::negative_underflow;
        }

        std::string_view buf { "xxxxxxxxxxx" };
        if(std::snprintf(const_cast<char*>(buf.data()), buf.size(), "%05.4f", value) < 0) {
            return exception_sevmaps::error;
        }
        if(check_snprintf_output(buf) == false) {
            return exception_sevmaps::error;
        }

        sevmap ret {};
        std::for_each(
            ret.rbegin(),
            ret.rend(),
            [&buf, index = ret.size()](auto& e) mutable {
                if(buf[index] == '-') {
                    e = minus_sign;
                } else if(buf[index] == '.') {
                    index--;
                    e = hex_map[numerical_char_to_uint8_t(buf[index])];
                    e |= dp_or_mask;
                } else {
                    e = hex_map[numerical_char_to_uint8_t(buf[index])];
                }
                index--;
            }
        );

        return ret;
    }
}
}
}