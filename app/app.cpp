#include "tasks/inc/temperature_control.hpp"
#include "tasks/inc/display_control.hpp"
#include "stm32f2xx_hal.h"
#include "tasks/inc/logger.hpp"

 void app_main(){
        tasks::temperature_control::cool_down();
        tasks::temperature_control::heat();
        // Other logic will be included here at the app level
}