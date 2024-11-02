#include "tasks/panel.hpp"
#include "panel/sevseg/white/white.hpp"
#include "panel/sevseg/common/common.hpp"

namespace tasks {
    Panel& Panel::get_instance() {
        static Panel instance {};
        return instance;
    }

    void Panel::worker(void* arg) {
        Panel& self { *static_cast<Panel*>(arg) };

        panel::sevseg::white::init_brightness();
        panel::sevseg::white::dim();

        float number { self.number };
        auto sevmap { panel::sevseg::common::float_to_sevmap(number) };
        while(1) {
            if(number != self.number) {
                number = self.number;
                sevmap = panel::sevseg::common::float_to_sevmap(number);
            }
            panel::sevseg::white::display_refresh(sevmap);
            osDelay(1);
        }
    }

    void Panel::increment() {
        if(number + step <= max) {
            number += step;
        }
    }

    void Panel::decrement() {
        if(number - step >= min) {
            number -= step;
        }
    }
}