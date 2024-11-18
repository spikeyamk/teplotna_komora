#pragma once

#include <serde/common.hpp>

namespace magic {
namespace commands {
    struct Connect {};
    struct Disconnect {};
    struct Nop {};

    struct WriteTemp {
        float value;
    };

    struct ReadTempCtl {};

    using Pack = Serde::pack_holder<
        Connect,
        Disconnect,
        Nop,
        WriteTemp,
        ReadTempCtl
    >;
}
}