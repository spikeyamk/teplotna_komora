#include <tuple>

#include "main.h"
#include "stm32f2xx_hal.h"
#include "panel/encoder/encoder.hpp"
#include "tasks/panel.hpp"

namespace panel {
namespace encoder {
    std::tuple<GPIO_PinState, GPIO_PinState> read_encoder_pins() {
        const GPIO_PinState enca { HAL_GPIO_ReadPin(ENCA_EXTI10_GPIO_Port, ENCA_EXTI10_Pin) };
        const GPIO_PinState encb { HAL_GPIO_ReadPin(ENCB_EXTI11_GPIO_Port, ENCB_EXTI11_Pin) };
        return std::make_tuple(enca, encb);
    }

    void enca_exti10_handler() {
        const auto [enca, encb] { read_encoder_pins() };
        if((enca == GPIO_PIN_SET) && (encb == GPIO_PIN_RESET)) {
            tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::Increment(), tasks::Panel::Timeout::IRQ);
        } else if((enca == GPIO_PIN_RESET) && (encb == GPIO_PIN_SET)) {
            tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::Increment(), tasks::Panel::Timeout::IRQ);
        } else if((enca == GPIO_PIN_RESET) && (encb == GPIO_PIN_RESET)) {
            tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::Decrement(), tasks::Panel::Timeout::IRQ);
        } else if((enca == GPIO_PIN_SET) && (encb == GPIO_PIN_SET)) {
            tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::Decrement(), tasks::Panel::Timeout::IRQ);
        }
    }

    void encb_exti11_handler() {
        const auto [enca, encb] { read_encoder_pins() };
        if((enca == GPIO_PIN_SET) && (encb == GPIO_PIN_RESET)) {
            tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::Decrement(), tasks::Panel::Timeout::IRQ);
        } else if((enca == GPIO_PIN_RESET) && (encb == GPIO_PIN_SET)) {
            tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::Decrement(), tasks::Panel::Timeout::IRQ);
        } else if((enca == GPIO_PIN_RESET) && (encb == GPIO_PIN_RESET)) {
            tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::Increment(), tasks::Panel::Timeout::IRQ);
        } else if((enca == GPIO_PIN_SET) && (encb == GPIO_PIN_SET)) {
            tasks::Panel::get_instance().push(tasks::Panel::Menu::Events::Increment(), tasks::Panel::Timeout::IRQ);
        }
    }
}
}