#pragma once

#include <serde/common.hpp>

namespace common {
namespace magic {
namespace results {
    struct WriteTemp {};

    struct ReadSensors {
        float temp_a;
        float temp_b;
        float fan_0;
        float fan_1;
        float fan_2;
        float fan_3;
        float fan_4;
        float fan_5;
        float fan_6;
    };

    using Pack = Serde::pack_holder<
        WriteTemp,
        ReadSensors
    >;
}
}
}