#include "i2c2/common.hpp"

namespace i2c2 {
    const uint32_t TIMEOUT { 10 };
    const GPIO_TypeDef* PORT { GPIOF };

    void Error_Handler() {
        /* USER CODE BEGIN Error_Handler_Debug */
        /* User can add his own implementation to report the HAL error return state */
        __disable_irq();
        while (1)
        {
        }
        /* USER CODE END Error_Handler_Debug */
    }
}