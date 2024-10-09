#include "stm32f2xx_hal.h"
#include "panel/sevseg/white/white.hpp"

namespace panel {
namespace sevseg {
namespace white {
    void init_brightness(TIM_HandleTypeDef* htim2) {
        HAL_TIM_PWM_Start(htim2, TIM_CHANNEL_1);
    }

    void dim(TIM_HandleTypeDef* htim2) {
        __HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_1, 50'000);
    }

    void bright(TIM_HandleTypeDef* htim2) {
        __HAL_TIM_SET_COMPARE(htim2, TIM_CHANNEL_1, 60'000);
    }

    void turn_on_all_segments() {
        //brightness
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
        
        //cathodes
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET); //4th digit
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);

        //segments
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);  //A
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);  //E
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);  //F
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET); //B
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET); //C
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET); //D
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET); //G
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET); //DP
    }

    void turn_off_all_segments() {        
        //assuming brightness has been initialized in turn_on_all_segments(), if not it needs to be initialized
        //enable all cathodes
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);

        //disable segments
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);  //A
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);  //E
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);  //F
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET); //B
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET); //C
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET); //D
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET); //G
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET); //DP

        //disable cathodes
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
    }

    void display_digit(uint32_t digit) {
        switch (digit) {
            case 0:
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
                break;
            case 1:
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
                break;
            case 2:
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
                break;
            case 3:
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
                break;
            case 4:
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
                break;
            case 5:
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
                break;
            case 6:
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
                break;
            case 7:
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
                break;
            case 8:
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
                break;
            case 9:
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
                break;
        }
    }

    void display_number(uint32_t number) {
        turn_off_all_segments();
        //numbers might be displayed in wrong order, need to test
        uint16_t active_cathode[5] = {
            GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4
        };

        for (uint8_t i = 0; (i < 5) && (number > 0); i++) {
            uint32_t digit = number % 10;
            number /= 10;
            HAL_GPIO_WritePin(GPIOE, active_cathode[i], GPIO_PIN_RESET);
            display_digit(digit);
        }

        /*HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);*/
    }
}
}
}