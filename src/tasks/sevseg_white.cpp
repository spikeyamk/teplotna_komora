#include "panel/sevseg/white/white.hpp"
#include "tasks/sevseg_white.hpp"

namespace tasks {
    SevsegWhite& SevsegWhite::get_instance() {
        static SevsegWhite instance {};
        return instance;
    }

    void SevsegWhite::worker(void* arg) {
        SevsegWhite& self { *static_cast<SevsegWhite*>(arg) };
        while(self.sevmap.has_value() == false) {
            osDelay(1);
        }
        auto sevmap { self.sevmap.value() };

        panel::sevseg::white::init();

        while(1) {
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

            if(self.sevmap.has_value()) {
                sevmap = self.consume();
            }

            panel::sevseg::white::refresh<[]() { util::microsec_blocking_delay(100); }>(sevmap);
            osDelay(1);
        }
    }

    void SevsegWhite::push(const panel::sevseg::common::sevmap& in_sevmap) {
        sevmap = in_sevmap;
    }

    panel::sevseg::common::sevmap SevsegWhite::consume() {
        auto ret { sevmap };
        sevmap = std::nullopt;
        return ret.value();
    }
}