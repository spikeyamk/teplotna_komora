#pragma once

#include <serde/common.hpp>

namespace common {
namespace magic {
namespace commands {
    struct WriteTemp {
        float value;
    };

    struct ReadSensors {};

    using Pack = Serde::pack_holder<
        WriteTemp,
        ReadSensors
    >;
}
}
}