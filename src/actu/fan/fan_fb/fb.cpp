#include <trielo/trielo.hpp>
#include "tim.h"

namespace actu {
namespace fan {
namespace fb {
namespace overflows {
    size_t tim5 { 0 };
    size_t tim4 { 0 };
    size_t tim3 { 0 };
}
    void init() {
        HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_2);
        HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_4);
        HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2);
        HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_4);
        HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
        HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);
    }
}
}
}