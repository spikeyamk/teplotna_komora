#include "actu/fan/ctl/ctl.hpp"
#include "util/util.hpp"
#include "tasks/senser_killer.hpp"
#include "tasks/temp_ctl.hpp"

namespace tasks {
    TempCtl& TempCtl::get_instance() {
        static TempCtl instance {};
        return instance;
    }

    void TempCtl::init() {
        actu::fan::ctl::all::init();
        util::turn_every_annoying_peripheral_off();
    }

    void TempCtl::worker(void* arg) {
        TempCtl& self { *static_cast<TempCtl*>(arg) };
        self.init();

        while(1) {
            std::printf("tasks::TempCtl::worker()\n");
            osDelay(1'000);
        }
    }
}