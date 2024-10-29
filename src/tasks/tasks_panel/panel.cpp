#include "tasks/panel.hpp"

namespace tasks {
    Panel& Panel::get_instance() {
        static Panel instance {};
        return instance;
    }

    void Panel::worker(void* arg) {
        Panel& self { *static_cast<Panel*>(arg) };
        while(1) {
            std::printf("tasks::Panel::worker: self.rpm: %f\n", self.rpm);
            osDelay(1'000);
        }
    }
}