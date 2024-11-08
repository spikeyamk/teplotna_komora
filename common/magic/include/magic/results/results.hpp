#pragma once

#include <serde/common.hpp>

namespace magic {
namespace results {
    struct Connect {};
    struct Disconnect {};
    struct Nop {};

    struct WriteTemp {
        float value;
    };

    struct ReadSensors {
        uint16_t temp_front;
        uint16_t temp_rear;
        uint16_t dac_front;
        uint16_t dac_rear;
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