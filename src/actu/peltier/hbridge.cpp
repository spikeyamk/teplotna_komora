#include "main.h"
#include "stm32f2xx_hal.h"
#include "hbridge.hpp"

namespace actu {
namespace peltier {
namespace hbridge {
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
        HAL_Delay(1);
        HAL_GPIO_WritePin(BRDGF_LLOW_GPIO_Port, BRDGF_LLOW_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BRDGF_RLOW_GPIO_Port, BRDGF_RLOW_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGF_LHIGH_GPIO_Port, BRDGF_LHIGH_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGF_RHIGH_GPIO_Port, BRDGF_RHIGH_Pin, GPIO_PIN_RESET);
    }

    void cool() {
        turn_off();
        HAL_Delay(1);
        HAL_GPIO_WritePin(BRDGF_LLOW_GPIO_Port, BRDGF_LLOW_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGF_RLOW_GPIO_Port, BRDGF_RLOW_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BRDGF_LHIGH_GPIO_Port, BRDGF_LHIGH_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BRDGF_RHIGH_GPIO_Port, BRDGF_RHIGH_Pin, GPIO_PIN_SET);
    }

    std::optional<State> get_state() {
        if(
            (HAL_GPIO_ReadPin(BRDGF_LLOW_GPIO_Port, BRDGF_LLOW_Pin) == GPIO_PIN_SET)
            && (HAL_GPIO_ReadPin(BRDGF_RLOW_GPIO_Port, BRDGF_RLOW_Pin) == GPIO_PIN_SET)
            && (HAL_GPIO_ReadPin(BRDGF_LHIGH_GPIO_Port, BRDGF_LHIGH_Pin) == GPIO_PIN_SET)
            && (HAL_GPIO_ReadPin(BRDGF_RHIGH_GPIO_Port, BRDGF_RHIGH_Pin) == GPIO_PIN_SET)
        ) {
            return State::Off;
        } else if(
            (HAL_GPIO_ReadPin(BRDGF_LLOW_GPIO_Port, BRDGF_LLOW_Pin) == GPIO_PIN_RESET)
            && (HAL_GPIO_ReadPin(BRDGF_RLOW_GPIO_Port, BRDGF_RLOW_Pin) == GPIO_PIN_SET)
            && (HAL_GPIO_ReadPin(BRDGF_LHIGH_GPIO_Port, BRDGF_LHIGH_Pin) == GPIO_PIN_SET)
            && (HAL_GPIO_ReadPin(BRDGF_RHIGH_GPIO_Port, BRDGF_RHIGH_Pin) == GPIO_PIN_RESET)
        ) {
            return State::Heat;
        } else if(
            (HAL_GPIO_ReadPin(BRDGF_LLOW_GPIO_Port, BRDGF_LLOW_Pin) == GPIO_PIN_SET)
            && (HAL_GPIO_ReadPin(BRDGF_RLOW_GPIO_Port, BRDGF_RLOW_Pin) == GPIO_PIN_RESET)
            && (HAL_GPIO_ReadPin(BRDGF_LHIGH_GPIO_Port, BRDGF_LHIGH_Pin) == GPIO_PIN_RESET)
            && (HAL_GPIO_ReadPin(BRDGF_RHIGH_GPIO_Port, BRDGF_RHIGH_Pin) == GPIO_PIN_SET)
        ) {
            return State::Cool;
        }
        
        return std::nullopt;
    }
}

namespace rear {
    void turn_off() {
        // Uses NOT gate should turn off with LOG_HIGH
        HAL_GPIO_WritePin(BRDGR_LLOW_GPIO_Port, BRDGR_LLOW_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGR_RLOW_GPIO_Port, BRDGR_RLOW_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGR_LHIGH_GPIO_Port, BRDGR_LHIGH_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGR_RHIGH_GPIO_Port, BRDGR_RHIGH_Pin, GPIO_PIN_SET);
    }

    void heat() {
        turn_off();
        HAL_Delay(1);
        HAL_GPIO_WritePin(BRDGR_LLOW_GPIO_Port, BRDGR_LLOW_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BRDGR_RLOW_GPIO_Port, BRDGR_RLOW_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGR_LHIGH_GPIO_Port, BRDGR_LHIGH_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGR_RHIGH_GPIO_Port, BRDGR_RHIGH_Pin, GPIO_PIN_RESET);
    }

    void cool() {
        turn_off();
        HAL_Delay(1);
        HAL_GPIO_WritePin(BRDGR_LLOW_GPIO_Port, BRDGR_LLOW_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(BRDGR_RLOW_GPIO_Port, BRDGR_RLOW_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BRDGR_LHIGH_GPIO_Port, BRDGR_LHIGH_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BRDGR_RHIGH_GPIO_Port, BRDGR_RHIGH_Pin, GPIO_PIN_SET);
    }

    std::optional<State> get_state() {
        if(
            (HAL_GPIO_ReadPin(BRDGR_LLOW_GPIO_Port, BRDGR_LLOW_Pin) == GPIO_PIN_SET)
            && (HAL_GPIO_ReadPin(BRDGR_RLOW_GPIO_Port, BRDGR_RLOW_Pin) == GPIO_PIN_SET)
            && (HAL_GPIO_ReadPin(BRDGR_LHIGH_GPIO_Port, BRDGR_LHIGH_Pin) == GPIO_PIN_SET)
            && (HAL_GPIO_ReadPin(BRDGR_RHIGH_GPIO_Port, BRDGR_RHIGH_Pin) == GPIO_PIN_SET)
        ) {
            return State::Off;
        } else if(
            (HAL_GPIO_ReadPin(BRDGR_LLOW_GPIO_Port, BRDGR_LLOW_Pin) == GPIO_PIN_RESET)
            && (HAL_GPIO_ReadPin(BRDGR_RLOW_GPIO_Port, BRDGR_RLOW_Pin) == GPIO_PIN_SET)
            && (HAL_GPIO_ReadPin(BRDGR_LHIGH_GPIO_Port, BRDGR_LHIGH_Pin) == GPIO_PIN_SET)
            && (HAL_GPIO_ReadPin(BRDGR_RHIGH_GPIO_Port, BRDGR_RHIGH_Pin) == GPIO_PIN_RESET)
        ) {
            return State::Heat;
        } else if(
            (HAL_GPIO_ReadPin(BRDGR_LLOW_GPIO_Port, BRDGR_LLOW_Pin) == GPIO_PIN_SET)
            && (HAL_GPIO_ReadPin(BRDGR_RLOW_GPIO_Port, BRDGR_RLOW_Pin) == GPIO_PIN_RESET)
            && (HAL_GPIO_ReadPin(BRDGR_LHIGH_GPIO_Port, BRDGR_LHIGH_Pin) == GPIO_PIN_RESET)
            && (HAL_GPIO_ReadPin(BRDGR_RHIGH_GPIO_Port, BRDGR_RHIGH_Pin) == GPIO_PIN_SET)
        ) {
            return State::Cool;
        }
        
        return std::nullopt;
    }

}
}
}
}