#include "panel/sevseg/common/common.hpp"
#include "panel/sevseg/common/test.hpp"

namespace panel {
namespace sevseg {
namespace common {
namespace test {
    int returns_zero() {
        return 0;
    }

    int should_fail() {
        float_to_sevmap(0.0001);
        if(returns_zero() == 0) {
            return -1;
        }

        return 0;
    }

    int should_pass() {
        if(returns_zero() != 0) {
            return -1;
        }

        return 0;
    }
}
}
}
}