#include "actu/fan/ctl/ctl.hpp"
#include "actu/pump/pump.hpp"
#include "actu/buzzer/buzzer.hpp"
#include "actu/bridge/bridge.hpp"
#include "util/util.hpp"

namespace util {
    void turn_every_annoying_peripheral_off() {
        actu::fan::ctl::init();
        actu::fan::ctl::stop_all();

        actu::pump::stop();
        actu::buzzer::stop();

        actu::bridge::front::turn_off();
        actu::bridge::rear::turn_off();
    }
}