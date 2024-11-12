#pragma once

#include <optional>
#include <bitint.hpp>

namespace actu {
namespace peltier {
    using int13_t = bitint::bitint<13>;
namespace front {
    void init();
    void set(const int13_t code);
    std::optional<int13_t> get_code();
}
namespace rear {
    void init();
    void set(const int13_t code);
    std::optional<int13_t> get_code();
}
}
}