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

    int float_to_sevmap() {
        if(Trielo::trielo<std::isnormal<float>>(std::nanf("1"))) {
            return -1;
        }

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
            const auto ret { Trielo::trielo<::panel::sevseg::common::float_to_sevmap>(expected_pairs[i].number) };
            if(Trielo::trielo<is_same>(ret, expected_pairs[i].expected_result) == false) {
                return static_cast<int>(i);
            }
        }

        return 0;
    }
}
}
}
}