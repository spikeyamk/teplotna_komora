#include "tasks/example.hpp"

namespace tasks {
    Example& Example::get_instance() {
        static Example instance {};
        return instance;
    }

    void Example::worker(void* arg) {
        Example& self { *static_cast<Example*>(arg) };
        while(1) {
            std::printf("tasks::Example::worker: self.rpm: %f\n", self.rpm);
            osDelay(1'000);
        }
    }
}