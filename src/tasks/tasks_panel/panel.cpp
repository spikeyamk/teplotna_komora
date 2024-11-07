#include "panel/sevseg/white/white.hpp"
#include "panel/sevseg/common/common.hpp"
#include "tasks/temp_senser.hpp"
#include "tasks/panel.hpp"

namespace tasks {
    Panel& Panel::get_instance() {
        static Panel instance {};
        return instance;
    }

    void Panel::worker(void* arg) {
        Panel& self { *static_cast<Panel*>(arg) };

        panel::sevseg::white::init_brightness();
        panel::sevseg::white::dim();

        sens::max31865::RTD desired_rtd { self.desired_rtd };
        auto sevmap { panel::sevseg::common::float_to_sevmap(desired_rtd.calculate_approx_temp().value()) };
        for(uint32_t tick = 0; true; tick++) {
            if(desired_rtd != self.desired_rtd) {
                desired_rtd = self.desired_rtd;
                sevmap = panel::sevseg::common::float_to_sevmap(desired_rtd.calculate_approx_temp().value());
            }
            panel::sevseg::white::display_refresh(sevmap);
            if(tick % 500 == 0) {
                self.max6549.yellow_show(TempSenser::get_instance().rtd_front.calculate_approx_temp().value_or(static_cast<float>(0xFF'FF'FF'FF)));
                self.max6549.green_show(TempSenser::get_instance().rtd_rear.calculate_approx_temp().value_or(static_cast<float>(0xFF'FF'FF'FF)));
            }
            osDelay(1);
        }
    }

    void Panel::increment() {
        //std::printf("tasks::Panel::increment\n");
        //std::cout
        //    << "desired_rtd.adc_code.value: "
        //    << desired_rtd.adc_code.value
        //    << "DESIRED_RTD_STEP.adc_code.value: "
        //    << DESIRED_RTD_STEP.adc_code.value
        //    << "DESIRED_RTD_MAX.adc_code.value: "
        //    << DESIRED_RTD_MAX.adc_code.value
        //    << std::endl;
        
        //std::cout
        //    << "desired_rtd.adc_code.value + DESIRED_RTD_STEP.adc_code.value: "
        //    << desired_rtd.adc_code.value + DESIRED_RTD_STEP.adc_code.value
        //    << std::endl;

        if((desired_rtd.adc_code.value + DESIRED_RTD_STEP.adc_code.value) <= DESIRED_RTD_MAX.adc_code.value) {
            //std::printf("tasks::Panel::increment: desired_rtd.adc_code.value + DESIRED_RTD_STEP.adc_code.value <= DESIRED_RTD_MAX.adc_code.value\n");
            desired_rtd.adc_code.value += DESIRED_RTD_STEP.adc_code.value;
        }
    }

    void Panel::decrement() {
        //std::printf("tasks::Panel::decrement\n");
        if((desired_rtd.adc_code.value - DESIRED_RTD_STEP.adc_code.value) >= DESIRED_RTD_MIN.adc_code.value) {
            //std::printf("desired_rtd.adc_code.value - DESIRED_RTD_STEP.adc_code.value >= DESIRED_RTD_MIN.adc_code.value\n");
            desired_rtd.adc_code.value -= DESIRED_RTD_STEP.adc_code.value;
        }
    }
}