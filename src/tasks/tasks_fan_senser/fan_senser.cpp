#include "tasks/fan_senser.hpp"

namespace tasks {
    FanSenser& FanSenser::get_instance() {
        static FanSenser instance {};
        return instance;
    }

    void FanSenser::worker(void* arg) {
        FanSenser& self { *static_cast<FanSenser*>(arg) };
        (void) self;
        while(1) {
            osDelay(1'000);
        }
    }
}