#include "tasks/temp_ctl.hpp"

namespace tasks {
    TempCtl& TempCtl::get_instance() {
        static TempCtl instance {};
        return instance;
    }

    void TempCtl::worker(void* arg) {
        TempCtl& self { *static_cast<TempCtl*>(arg) };
        while(1) {
            std::printf("tasks::TempCtl::worker: self.rpm: %f\n", self.rpm);
            osDelay(1'000);
        }
    }
}