#include "main.h"
#include "stm32f2xx_hal.h"
#include "panel/encoder/encoder.hpp"
#include "tasks/panel.hpp"

namespace panel {
namespace encoder {
    void enca_ext10_handler() {
        const GPIO_PinState encoder_A = HAL_GPIO_ReadPin(ENCA_EXTI10_GPIO_Port, ENCA_EXTI10_Pin);
        const GPIO_PinState encoder_B = HAL_GPIO_ReadPin(ENCB_EXTI11_GPIO_Port, ENCB_EXTI11_Pin);
        if ((encoder_A == GPIO_PIN_SET) && (encoder_B == GPIO_PIN_RESET))
            tasks::Panel::get_instance().increment();
        else if ((encoder_A == GPIO_PIN_RESET) && (encoder_B == GPIO_PIN_SET))
            tasks::Panel::get_instance().increment();
        else if ((encoder_A == GPIO_PIN_RESET) && (encoder_B == GPIO_PIN_RESET))
            tasks::Panel::get_instance().decrement();
        else if ((encoder_A == GPIO_PIN_SET) && (encoder_B == GPIO_PIN_SET))
            tasks::Panel::get_instance().decrement();
    }
    void encb_ext11_handler() {
        const GPIO_PinState encoder_A = HAL_GPIO_ReadPin(ENCA_EXTI10_GPIO_Port, ENCA_EXTI10_Pin);
        const GPIO_PinState encoder_B = HAL_GPIO_ReadPin(ENCB_EXTI11_GPIO_Port, ENCB_EXTI11_Pin);
        if ((encoder_A == GPIO_PIN_SET) && (encoder_B == GPIO_PIN_RESET))
            tasks::Panel::get_instance().decrement();
        else if ((encoder_A == GPIO_PIN_RESET) && (encoder_B == GPIO_PIN_SET))
            tasks::Panel::get_instance().decrement();
        else if ((encoder_A == GPIO_PIN_RESET) && (encoder_B == GPIO_PIN_RESET))
            tasks::Panel::get_instance().increment();
        else if ((encoder_A == GPIO_PIN_SET) && (encoder_B == GPIO_PIN_SET))
            tasks::Panel::get_instance().increment();
    }
}
}