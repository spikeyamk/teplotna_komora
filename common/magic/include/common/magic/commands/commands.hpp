#pragma once

#include <serde/common.hpp>

namespace common {
namespace magic {
namespace commands {
    struct Connect {};
    struct Disconnect {};
    struct Nop {};

    struct WriteTemp {
        float value;
    };

    struct ReadSensors {};


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