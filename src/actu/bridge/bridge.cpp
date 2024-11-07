#include <iostream>
#include "main.h"
#include "cmsis_os2.h"
#include "stm32f2xx_hal.h"
#include "actu/bridge/bridge.hpp"

namespace actu {
namespace bridge {
namespace front {
    void turn_off() {
        // Uses NOT gate should turn off with LOG_HIGH
        HAL_GPIO_WritePin(BRDGF_LLOW_GPIO_Port, BRDGF_LLOW_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGF_RLOW_GPIO_Port, BRDGF_RLOW_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGF_LHIGH_GPIO_Port, BRDGF_LHIGH_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGF_RHIGH_GPIO_Port, BRDGF_RHIGH_Pin, GPIO_PIN_SET);
    }

    void heat() {
        turn_off();
        osDelayUntil(1);
        HAL_GPIO_WritePin(BRDGF_LLOW_GPIO_Port, BRDGF_LLOW_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BRDGF_RLOW_GPIO_Port, BRDGF_RLOW_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGF_LHIGH_GPIO_Port, BRDGF_LHIGH_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGF_RHIGH_GPIO_Port, BRDGF_RHIGH_Pin, GPIO_PIN_RESET);
    }

    void cool() {
        turn_off();
        osDelayUntil(1);
        HAL_GPIO_WritePin(BRDGF_LLOW_GPIO_Port, BRDGF_LLOW_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGF_RLOW_GPIO_Port, BRDGF_RLOW_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BRDGF_LHIGH_GPIO_Port, BRDGF_LHIGH_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BRDGF_RHIGH_GPIO_Port, BRDGF_RHIGH_Pin, GPIO_PIN_SET);
    }
}

namespace rear {
    void turn_off() {
        // Uses NOT gate should turn off with LOG_HIGH
        HAL_GPIO_WritePin(BRDGR_RLOW_GPIO_Port, BRDGR_RLOW_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGR_LLOW_GPIO_Port, BRDGR_LLOW_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGR_RHIGH_GPIO_Port, BRDGR_RHIGH_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGR_LHIGH_GPIO_Port, BRDGR_LHIGH_Pin, GPIO_PIN_SET);
    }

    void heat() {
        turn_off();
        osDelayUntil(1);
        HAL_GPIO_WritePin(BRDGR_RLOW_GPIO_Port, BRDGR_RLOW_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGR_LLOW_GPIO_Port, BRDGR_LLOW_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BRDGR_RHIGH_GPIO_Port, BRDGR_RHIGH_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BRDGR_LHIGH_GPIO_Port, BRDGR_LHIGH_Pin, GPIO_PIN_SET);
    }

    void cool() {
        turn_off();
        osDelayUntil(1);
        HAL_GPIO_WritePin(BRDGR_RLOW_GPIO_Port, BRDGR_RLOW_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BRDGR_LLOW_GPIO_Port, BRDGR_LLOW_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGR_RHIGH_GPIO_Port, BRDGR_RHIGH_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGR_LHIGH_GPIO_Port, BRDGR_LHIGH_Pin, GPIO_PIN_RESET);
    }
}

namespace control {
    bool turn_off_peltier(bool sel_peltier) {
        if (sel_peltier == true) {
            rear::turn_off();
            std::printf("Rear peltier turned off. Front peltier selected.\n");
        }
        else if (sel_peltier == false) {
            front::turn_off();
            std::printf("Front peltier turned off. Rear peltier selected.\n");
        }
        
        return !sel_peltier;
    }

    bool heat_or_cool(bool sel_peltier, bool heat_or_cool) {
        if (sel_peltier == true && heat_or_cool == true) {
            front::heat();
            std::printf("Front peltier is now heating.\n");
        }
        else if (sel_peltier == true && heat_or_cool == false) {
            front::cool();
            std::printf("Front peltier is now cooling.\n");
        }
        else if (sel_peltier == false && heat_or_cool == true) {
            rear::heat();
            std::printf("Back peltier is now heating.\n");
        }
        else if (sel_peltier == false && heat_or_cool == false) {
            rear::cool();
            std::printf("Back peltier is now cooling.\n");
        }
        
        return !heat_or_cool;
    }

    bool heat_or_cool_all(bool heat_or_cool) {
        if (heat_or_cool == true) {
            front::heat();
            rear::heat();
            std::printf("Heating both peltiers.\n");
        }
        else if (heat_or_cool == false) {
            front::heat();
            rear::heat();
            std::printf("Cooling both peltiers.\n");
        }

        return !heat_or_cool;
    }
}
}
}