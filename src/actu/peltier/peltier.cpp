#include <cmath>

#include "hbridge.hpp"
#include "current_source.hpp"
#include "actu/peltier/peltier.hpp"

namespace actu {
namespace peltier {
namespace front {
    void init() {
        current_source::front::init();
    }

    void set(const int13_t code) {
        if(code == int13_t(0)) {
            hbridge::front::turn_off();
        } else if(code > int13_t(0)) {
            hbridge::front::heat();
        } else if(code < int13_t(0)) {
            hbridge::front::cool();
        }

        current_source::front::set(current_source::uint12_t(static_cast<uint16_t>(std::abs(code.unwrap()))));
    }

    std::optional<int13_t> get_code() {
        const current_source::uint12_t current_source_code { current_source::front::get_code() };
        const std::optional<hbridge::State> hbridge_state { hbridge::front::get_state() };

        if(hbridge_state.has_value() == false) {
            return std::nullopt;
        }

        if(hbridge_state.value() == hbridge::State::Cool) {
            return int13_t( - static_cast<int16_t>(current_source_code.unwrap()));
        } else if(hbridge_state.value() == hbridge::State::Heat) {
            return int13_t(static_cast<int16_t>(current_source_code.unwrap()));
        } else if(
            (hbridge_state.value() == hbridge::State::Off)
            && (current_source_code == current_source::uint12_t(0))
        ) {
            return int13_t(0);
        }

        return std::nullopt;
    }
}

namespace rear {
    void init() {
        current_source::rear::init();
    }
    
    void set(const int13_t code) {
        if(code == int13_t(0)) {
            hbridge::rear::turn_off();
        } else if(code > int13_t(0)) {
            hbridge::rear::heat();
        } else if(code < int13_t(0)) {
            hbridge::rear::cool();
        }

        current_source::rear::set(current_source::uint12_t(static_cast<uint16_t>(std::abs(code.unwrap()))));
    }

    std::optional<int13_t> get_code() {
        const current_source::uint12_t current_source_code { current_source::rear::get_code() };
        const std::optional<hbridge::State> hbridge_state { hbridge::rear::get_state() };

        if(hbridge_state.has_value() == false) {
            return std::nullopt;
        }

        if(hbridge_state.value() == hbridge::State::Cool) {
            return int13_t( - static_cast<int16_t>(current_source_code.unwrap()));
        } else if(hbridge_state.value() == hbridge::State::Heat) {
            return int13_t(static_cast<int16_t>(current_source_code.unwrap()));
        } else if(
            (hbridge_state.value() == hbridge::State::Off)
            && (current_source_code == current_source::uint12_t(0))
        ) {
            return int13_t(0);
        }

        return std::nullopt;
    }
}
}
}