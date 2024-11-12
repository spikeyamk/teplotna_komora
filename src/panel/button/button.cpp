#include "main.h"
#include "stm32f2xx_hal.h"
#include "tasks/panel.hpp"
#include "panel/button/button.hpp"

namespace panel {
namespace button {
    void but0_fr_handler() {

    }

    void but1_mr_handler() {
        if(HAL_GPIO_ReadPin(BUT1_MR_GPIO_Port, BUT1_MR_Pin) == GPIO_PIN_SET) {
            tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::StartBlink(), tasks::Panel::Timeout::IRQ);
        } else {
            tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::StopBlink(), tasks::Panel::Timeout::IRQ);
        }
    }

    void but2_ml_handler() {
        tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::Next(), tasks::Panel::Timeout::IRQ);
    }

    void but3_fl_handler() {
        tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::TurnOn(), tasks::Panel::Timeout::IRQ);
    }
}
}