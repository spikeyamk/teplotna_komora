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

        float number { self.desired_temp };
        auto sevmap { panel::sevseg::common::float_to_sevmap(number) };
        while(1) {
            if(number != self.desired_temp) {
                number = self.desired_temp;
                sevmap = panel::sevseg::common::float_to_sevmap(number);
            }
            panel::sevseg::white::display_refresh(sevmap);
            osDelay(1);
        }
    }

    void Panel::increment() {
        if(desired_temp + step <= max) {
            desired_temp += step;
        }
    }

    void Panel::decrement() {
        if(desired_temp - step >= min) {
            desired_temp -= step;
        }
    }
}