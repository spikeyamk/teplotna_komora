#pragma once

namespace actu {
namespace bridge {
namespace front {
    void turn_off();
    void heat();
    void cool();
}

namespace rear {
    void turn_off();
    void heat();
    void cool();
}

namespace control {
    bool turn_off_peltier(bool sel_peltier);
    bool heat_or_cool(bool sel_peltier, bool heat_or_cool);
    bool heat_or_cool_all(bool heat_or_cool);
}
}
}