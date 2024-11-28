#include <cmath>
#include <vector>
#include <algorithm>
#include <trielo/trielo.hpp>
#include "panel/sevseg/common/common.hpp"
#include "panel/sevseg/common/test.hpp"

std::ostream& operator<<(std::ostream& os, const panel::sevseg::common::sevmap& obj) {
    os << "sevmap: " << reinterpret_cast<const void*>(&obj) << std::endl;
    for(size_t i = 0; i < obj.size(); i++) {
        os
            << "obj["
            << i
            << "]: "
            << obj[i]
            << std::endl;
    }
    return os;
}

namespace panel {
namespace sevseg {
namespace common {
namespace test {
    inline bool is_same(const sevmap& rhs, const sevmap& lhs) {
        return rhs == lhs;
    }

    void print(const common::sevmap& sevmap, const common::sevmap& expected_sevmap) {
        for(size_t i = 0; i < sevmap.size(); i++) {
            std::cout
                << "sevmap["
                << i
                << "]: "
                << sevmap[i]
                << std::endl;
            std::cout
                << "expected_sevmap["
                << i
                << "]: "
                << expected_sevmap[i]
                << std::endl;
        }
    }

    int float_to_sevmap() {
        struct ExpectedPair {
            float number;
            sevmap expected_result;
        };

        const std::vector<ExpectedPair> expected_pairs {{
            // exceptions
            {std::nanf("1"), exception_sevmap::error},
            {99999.9f, exception_sevmap::positive_overflow},
            {-9999.9f, exception_sevmap::negative_overflow},
            {0.00001f, exception_sevmap::positive_underflow},
            {-0.0001f, exception_sevmap::negative_underflow},

            // positive values
            {0.0001f, sevmap{{hex_map[0] | dp_or_mask, hex_map[0], hex_map[0], hex_map[0], hex_map[1]}}},
            {0.0010f, sevmap{{hex_map[0] | dp_or_mask, hex_map[0], hex_map[0], hex_map[1], hex_map[0]}}},
            {0.0100f, sevmap{{hex_map[0] | dp_or_mask, hex_map[0], hex_map[1], hex_map[0], hex_map[0]}}},
            {0.1000f, sevmap{{hex_map[0] | dp_or_mask, hex_map[1], hex_map[0], hex_map[0], hex_map[0]}}},
            {1.0000f, sevmap{{hex_map[1] | dp_or_mask, hex_map[0], hex_map[0], hex_map[0], hex_map[0]}}},
            {10.000f, sevmap{{hex_map[1], hex_map[0] | dp_or_mask, hex_map[0], hex_map[0], hex_map[0]}}},
            {100.00f, sevmap{{hex_map[1], hex_map[0], hex_map[0] | dp_or_mask, hex_map[0], hex_map[0]}}},
            {1000.0f, sevmap{{hex_map[1], hex_map[0], hex_map[0], hex_map[0] | dp_or_mask, hex_map[0]}}},

            // positive values two different digits
            {0.0091f, sevmap{{hex_map[0] | dp_or_mask, hex_map[0], hex_map[0], hex_map[9], hex_map[1]}}},
            {0.0910f, sevmap{{hex_map[0] | dp_or_mask, hex_map[0], hex_map[9], hex_map[1], hex_map[0]}}},
            {0.9100f, sevmap{{hex_map[0] | dp_or_mask, hex_map[9], hex_map[1], hex_map[0], hex_map[0]}}},
            {9.1000f, sevmap{{hex_map[9] | dp_or_mask, hex_map[1], hex_map[0], hex_map[0], hex_map[0]}}},
            {91.000f, sevmap{{hex_map[9], hex_map[1] | dp_or_mask, hex_map[0], hex_map[0], hex_map[0]}}},
            {910.00f, sevmap{{hex_map[9], hex_map[1], hex_map[0] | dp_or_mask, hex_map[0], hex_map[0]}}},
            {9100.0f, sevmap{{hex_map[9], hex_map[1], hex_map[0], hex_map[0] | dp_or_mask, hex_map[0]}}},

            // negative values
            {-0.001f, sevmap{{minus_sign, hex_map[0] | dp_or_mask, hex_map[0], hex_map[0], hex_map[1]}}},
            {-0.010f, sevmap{{minus_sign, hex_map[0] | dp_or_mask, hex_map[0], hex_map[1], hex_map[0]}}},
            {-0.100f, sevmap{{minus_sign, hex_map[0] | dp_or_mask, hex_map[1], hex_map[0], hex_map[0]}}},
            {-1.000f, sevmap{{minus_sign, hex_map[1] | dp_or_mask, hex_map[0], hex_map[0], hex_map[0]}}},
            {-10.00f, sevmap{{minus_sign, hex_map[1], hex_map[0] | dp_or_mask, hex_map[0], hex_map[0]}}},
            {-100.0f, sevmap{{minus_sign, hex_map[1], hex_map[0], hex_map[0] | dp_or_mask, hex_map[0]}}},

            // negative values two different digits
            {-0.091f, sevmap{{minus_sign, hex_map[0] | dp_or_mask, hex_map[0], hex_map[9], hex_map[1]}}},
            {-0.910f, sevmap{{minus_sign, hex_map[0] | dp_or_mask, hex_map[9], hex_map[1], hex_map[0]}}},
            {-9.100f, sevmap{{minus_sign, hex_map[9] | dp_or_mask, hex_map[1], hex_map[0], hex_map[0]}}},
            {-91.00f, sevmap{{minus_sign, hex_map[9], hex_map[1] | dp_or_mask, hex_map[0], hex_map[0]}}},
            {-910.0f, sevmap{{minus_sign, hex_map[9], hex_map[1], hex_map[0] | dp_or_mask, hex_map[0]}}},
        }};

        for(size_t i = 0; i < expected_pairs.size(); i++) {
            const auto ret { ::panel::sevseg::common::to_sevmap<float>(expected_pairs[i].number) };
            if(ret != expected_pairs[i].expected_result) {
                print(ret, expected_pairs[i].expected_result);
                return static_cast<int>(i - 1);
            }
        }

        return 0;
    }


    int uint20_t_to_sevmap() {
        const auto sevmap { common::to_sevmap(uint20_t(0xA'E1'55)) };
        const common::sevmap expected_sevmap {
            common::hex_map[0xA],
            common::hex_map[0xE],
            common::hex_map[0x1],
            common::hex_map[0x5],
            common::hex_map[0x5]
        };

        if(sevmap != expected_sevmap) {
            print(sevmap, expected_sevmap);
            return 1;
        }

        return 0;
    }

    int string_to_sevmap() {
        {
            const auto sevmap { common::to_sevmap<"t">() };
            const common::sevmap expected_map = {
                common::latin_map['t' - 'a'],
                common::sevset{},
                common::sevset{},
                common::sevset{},
                common::sevset{}
            };

            if(sevmap != expected_map) {
                print(sevmap, expected_map);
                return 1;
            }
        }

        {
            const auto sevmap { common::to_sevmap<"te">() };
            const common::sevmap expected_map {
                common::latin_map['t' - 'a'],
                common::latin_map['e' - 'a'],
                common::sevset{},
                common::sevset{},
                common::sevset{}
            };

            if(sevmap != expected_map) {
                print(sevmap, expected_map);
                return 2;
            }
        }

        {
            const auto sevmap { common::to_sevmap<"tes">() };
            const common::sevmap expected_sevmap {
                common::latin_map['t' - 'a'],
                common::latin_map['e' - 'a'],
                common::latin_map['s' - 'a'],
                common::sevset{},
                common::sevset{}
            };

            if(sevmap != expected_sevmap) {
                print(sevmap, expected_sevmap);
                return 3;
            }
        }

        {
            const auto sevmap { common::to_sevmap<"test">() };
            const common::sevmap expected_sevmap {
                common::latin_map['t' - 'a'],
                common::latin_map['e' - 'a'],
                common::latin_map['s' - 'a'],
                common::latin_map['t' - 'a'],
                common::sevset{}
            };

            if(sevmap != expected_sevmap) {
                print(sevmap, expected_sevmap);
                return 4;
            }
        }

        {
            const auto sevmap { common::to_sevmap<"teste">() };
            const common::sevmap expected_sevmap {
                common::latin_map['t' - 'a'],
                common::latin_map['e' - 'a'],
                common::latin_map['s' - 'a'],
                common::latin_map['t' - 'a'],
                common::latin_map['e' - 'a']
            };

            if(sevmap != expected_sevmap) {
                print(sevmap, expected_sevmap);
                return 5;
            }
        }

        return 0;
    }

    int int14_t_to_sevmap() {
        {
            const auto sevmap { common::to_sevmap(common::int14_t(4)) };
            const common::sevmap expected_map = {
                common::hex_map[4],
                common::sevset{},
                common::sevset{},
                common::sevset{},
                common::sevset{}
            };

            if(sevmap != expected_map) {
                print(sevmap, expected_map);
                return 1;
            }
        }
        
        {
            const auto sevmap { common::to_sevmap(common::int14_t(40)) };
            const common::sevmap expected_map = {
                common::hex_map[4],
                common::hex_map[0],
                common::sevset{},
                common::sevset{},
                common::sevset{}
            };

            if(sevmap != expected_map) {
                print(sevmap, expected_map);
                return 2;
            }
        }

        {
            const auto sevmap { common::to_sevmap(common::int14_t(409)) };
            const common::sevmap expected_map = {
                common::hex_map[4],
                common::hex_map[0],
                common::hex_map[9],
                common::sevset{},
                common::sevset{}
            };

            if(sevmap != expected_map) {
                print(sevmap, expected_map);
                return 3;
            }
        }

        {
            const auto sevmap { common::to_sevmap(common::int14_t(4095)) };
            const common::sevmap expected_map = {
                common::hex_map[4],
                common::hex_map[0],
                common::hex_map[9],
                common::hex_map[5],
                common::sevset{}
            };

            if(sevmap != expected_map) {
                print(sevmap, expected_map);
                return 4;
            }
        }

        {
            const auto sevmap { common::to_sevmap(common::int14_t(common::int14_t::max)) };
            const common::sevmap expected_map = {
                common::hex_map[8],
                common::hex_map[1],
                common::hex_map[9],
                common::hex_map[1],
                common::sevset{}
            };

            if(sevmap != expected_map) {
                print(sevmap, expected_map);
                return 5;
            }
        }

        {
            const auto sevmap { common::to_sevmap(common::int14_t(common::int14_t::min)) };
            std::cout
                << "common::int14_t::min"
                << common::int14_t::min
                << std::endl;
            const common::sevmap expected_map = {
                common::minus_sign,
                common::hex_map[8],
                common::hex_map[1],
                common::hex_map[9],
                common::hex_map[2],
            };

            if(sevmap != expected_map) {
                print(sevmap, expected_map);
                return 6;
            }
        }

        return 0;
    }
}
}
}
}