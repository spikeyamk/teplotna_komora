#include "actu/fan/ctl/ctl.hpp"
#include "actu/bridge/bridge.hpp"
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
        actu::lin_source::front::start_dac();
        actu::lin_source::rear::start_dac();

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

        set_dac_front(4095);
        set_dac_rear(4095);

        actu::bridge::front::heat();
        actu::bridge::rear::heat();
    }

    void TempCtl::cool_full_power() {
        actu::fan::ctl::all::start_full_speed();

        set_dac_front(4095);
        set_dac_rear(4095);

        actu::bridge::front::cool();
        actu::bridge::rear::cool();
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
            if(SenserKiller::get_instance().rtd_front.adc_code.value >= Panel::DESIRED_RTD_MAX.adc_code.value) {
                self.cool_full_power();
            } else if(SenserKiller::get_instance().rtd_front.adc_code.value <= Panel::DESIRED_RTD_MIN.adc_code.value) {
                self.heat_full_power();
            }
            osDelay(1'000);
        }
    }

    void TempCtl::release_semaphore() {
        osSemaphoreRelease(semaphore);
    }
    
    void TempCtl::set_dac_front(const actu::lin_source::uint12_t value) {
        dac_front = value;
        actu::lin_source::front::set_output(value);
    }
    
    void TempCtl::set_dac_rear(const actu::lin_source::uint12_t value) {
        dac_rear = value;
        actu::lin_source::rear::set_output(value);
    }
}