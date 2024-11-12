#pragma once

#include <optional>

namespace actu {
namespace peltier {
namespace hbridge {
    enum class State {
        Off,
        Heat,
        Cool,
    };
namespace front {
    void turn_off();
    void heat();
    void cool();
    std::optional<State> get_state();
}

namespace rear {
    void turn_off();
    void heat();
    void cool();
    std::optional<State> get_state();
}
}
}
}