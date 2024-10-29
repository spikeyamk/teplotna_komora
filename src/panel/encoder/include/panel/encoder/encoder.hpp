#pragma once
#include <iostream>
#include "stm32f2xx_hal.h"

extern float counter;
#ifdef __cplusplus
extern "C" {
#endif
namespace panel {
namespace encoder {
    //void encoder_test();
    void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
}
}
#ifdef __cplusplus
}
#endif

/*namespace panel {
namespace encoder {
    //void encoder_test();
    extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
}
}*/