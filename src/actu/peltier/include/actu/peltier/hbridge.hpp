#pragma once

#include <iostream>

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
    void set_state(const State state);
    State get_state();
}

namespace rear {
    void turn_off();
    void heat();
    void cool();
    void set_state(const State state);
    State get_state();
}
}
}
}

std::ostream& operator<<(std::ostream& os, const actu::peltier::hbridge::State& obj);