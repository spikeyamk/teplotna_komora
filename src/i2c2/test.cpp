#include "i2c2/i2c2.hpp"
#include "i2c2/test.hpp"

namespace i2c2 {
    namespace hw {
        void test() {
            Bus().scan();
        }
    }

    namespace sw {
        void test() {
            Bus bus;
            //bus.
            //Bus().scan();
        }
    }
}