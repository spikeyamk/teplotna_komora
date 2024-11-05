#include "common/magic/test.hpp"

namespace common {
namespace magic {
namespace test {
    int returns_zero() {
        return 0;
    }

    int should_fail() {
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