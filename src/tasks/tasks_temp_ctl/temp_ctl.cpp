#include "actu/peltier/peltier.hpp"
#include "util/util.hpp"
#include "tasks/senser_killer.hpp"
#include "tasks/panel.hpp"
#include "tasks/temp_ctl.hpp"

namespace tasks {
    TempCtl& TempCtl::get_instance() {
        static TempCtl instance {};
        return instance;
    }

    void TempCtl::init() {
        actu::fan::ctl::all::init();
        util::turn_every_annoying_peripheral_off();
        actu::peltier::front::init();
        actu::peltier::rear::init();

        const osSemaphoreAttr_t sempahore_attr {
            .name = "temp_ctl_sem",
            .attr_bits = 0,
            .cb_mem = &semaphore_control_block,
            .cb_size = sizeof(semaphore_control_block),
        };

        semaphore = osSemaphoreNew(1, 0, &sempahore_attr);
    }
    
    void TempCtl::heat_full_power() {
        actu::fan::ctl::all::start_full_speed();

        actu::peltier::front::set(+4095);
        actu::peltier::rear::set(+4095);
    }

    void TempCtl::cool_full_power() {
        actu::fan::ctl::all::start_full_speed();

        actu::peltier::front::set(-4095);
        actu::peltier::rear::set(-4095);
    }

    void TempCtl::worker(void* arg) {
        TempCtl& self { *static_cast<TempCtl*>(arg) };
        self.init();

        if(osSemaphoreAcquire(self.semaphore, osWaitForever) != osOK) {
            std::printf("tasks::TempCtl::worker(): osSemaphoreAcquire(self.semaphore, osWaitForever) != osOK\n");
        }

        std::printf("tasks::TempCtl::worker(): semaphore acquired\n");

        self.heat_full_power();

        while(1) {
            /*
            if(SenserKiller::get_instance().rtd_front.adc_code.value >= Panel::DESIRED_RTD_MAX.adc_code.value) {
                self.cool_full_power();
            } else if(SenserKiller::get_instance().rtd_front.adc_code.value <= Panel::DESIRED_RTD_MIN.adc_code.value) {
                self.heat_full_power();
            }
            */
            osDelay(1'000);
        }
    }

    void TempCtl::release_semaphore() {
        osSemaphoreRelease(semaphore);
    }
}