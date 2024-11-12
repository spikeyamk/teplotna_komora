#include "panel/sevseg/white/white.hpp"
#include "tasks/sevseg_white.hpp"

namespace tasks {
    SevsegWhite& SevsegWhite::get_instance() {
        static SevsegWhite instance {};
        return instance;
    }

    void SevsegWhite::worker(void* arg) {
        SevsegWhite& self { *static_cast<SevsegWhite*>(arg) };
        /*
        while(self.sevmap.has_value() == false) {
            osDelay(1);
        }
        */
        auto sevmap { panel::sevseg::common::to_sevmap<"ahoj">() };
        std::printf("auto sevmap { self.consume() };\n");
        for(size_t i = 0; i < sevmap.size(); i++) {
            std::cout
                << "sevmap[" << i << "]"
                << sevmap[i]
                << std::endl;
        }

        panel::sevseg::white::init();

        while(1) {
            /*
            while(self.blinking) {
                for(uint32_t i = 0; (i < BLINK_TICKS) && (self.blinking == true); i++) {
                    if(self.sevmap.has_value()) {
                        sevmap = self.consume();
                    }

                    panel::sevseg::white::refresh<[]() { util::microsec_blocking_delay(100); }>(sevmap);
                    osDelay(1);
                }

                for(uint32_t i = 0; (i < BLINK_TICKS) && (self.blinking == true); i++) {
                    if(self.sevmap.has_value()) {
                        sevmap = self.consume();
                    }

                    osDelay(1);
                }
            }
            */

            /*
            if(self.sevmap.has_value()) {
                sevmap = self.consume();
            }
            */

            panel::sevseg::white::refresh<[]() { util::microsec_blocking_delay(100); }>(sevmap);
            osDelay(1);
        }
    }

    void SevsegWhite::push(const panel::sevseg::common::sevmap& in_sevmap) {
        std::printf("tasks::SevsegWhite::push(const panel::sevseg::common::sevmap& in_sevmap)\n");
        for(size_t i = 0; i < in_sevmap.size(); i++) {
            std::cout
                << "in_sevmap[" << i << "]: "
                << in_sevmap[i]
                << std::endl;
        }
        taskDISABLE_INTERRUPTS();
        sevmap = in_sevmap;
        taskENABLE_INTERRUPTS();
    }

    panel::sevseg::common::sevmap SevsegWhite::consume() {
        std::printf("panel::sevseg::common::sevmap tasks::SevsegWhite::consume()\n");
        taskDISABLE_INTERRUPTS();
        auto ret { sevmap };
        sevmap = std::nullopt;
        taskENABLE_INTERRUPTS();

        for(size_t i = 0; i < ret.value().size(); i++) {
            std::cout
                << "ret.value()[" << i << "]: "
                << ret.value()[i]
                << std::endl;
        }

        return ret.value();
    }
}