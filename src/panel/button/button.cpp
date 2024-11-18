#include "main.h"
#include "stm32f2xx_hal.h"
#include "tasks/panel.hpp"
#include "panel/button/button.hpp"

namespace panel {
namespace button {
    void but0_fr_handler() {
        tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::NextStep(), tasks::Panel::Timeout::IRQ);
    }

    void but1_mr_handler() {
        tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::BlinkToggle(), tasks::Panel::Timeout::IRQ);
    }

    void but2_ml_handler() {
        tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::NextItem(), tasks::Panel::Timeout::IRQ);
    }

    void but3_fl_handler() {
        if(tasks::Panel::get_instance().on_off) {
            tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::TurnOff(), tasks::Panel::Timeout::IRQ);
        } else {
            tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::TurnOn(), tasks::Panel::Timeout::IRQ);
        }
    }
}
}