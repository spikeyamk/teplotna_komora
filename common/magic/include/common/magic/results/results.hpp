#pragma once

#include <serde/common.hpp>

namespace common {
namespace magic {
namespace results {
    struct Connect {};
    struct Disconnect {};
    struct Nop {};

    struct WriteTemp {
        float value;
    };

    struct ReadSensors {
        float temp_front;
        float temp_rear;
    };

    using Pack = Serde::pack_holder<
        Connect,
        Disconnect,
        Nop,
        WriteTemp,
        ReadSensors
    >;
}
}
}