#include <array>
#include <bitset>
#include <iostream>
#include <cmath>
#include "stm32f2xx_hal.h"
#include "panel/sevseg/white/white.hpp"
#include "panel/sevseg/common/common.hpp"
#include "tim.h"

namespace panel {
namespace sevseg {
namespace white {
    void init_brightness() {
        HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    }

    void dim() {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 50'000);
    }

    void bright() {
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 60'000);
    }

    void brightness_on() {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
    }

    void brightness_off() {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
    }

    void turn_on_all_segments() {
        brightness_on();

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

    void display_pins() {
        const std::array<uint16_t, 8> active_segment {
            GPIO_PIN_13, //stred
            GPIO_PIN_9,  //vlavo hore
            GPIO_PIN_8,  //vlavo dole
            GPIO_PIN_12, //dole
            GPIO_PIN_11, //vpravo dole
            GPIO_PIN_10, //vpravo hore
            GPIO_PIN_7,  //hore
            GPIO_PIN_14  //decimal point
        };
        
        const std::array<uint16_t, 5> active_cathodes {
            GPIO_PIN_1, 
            GPIO_PIN_0, 
            GPIO_PIN_2, 
            GPIO_PIN_3, 
            GPIO_PIN_4
        };
    }

    uint8_t set_digit(float number, uint8_t position) {
        uint8_t digit = 0;
        for (;position > 0; position--){
            digit = fmod(number, 10);
        }
        return digit;
    }
    
    void display_refresh(float number) {
        uint8_t Mask = 0b1000'0000;
        uint8_t CurrentDigit = 0;
        static uint8_t RefreshedDigit = 0;
        
        //turn off last cathode
        HAL_GPIO_WritePin(GPIOE, active_cathodes[RefreshedDigit], GPIO_PIN_SET);
        
        //go to next digit
        RefreshedDigit++;
        if (RefreshedDigit >= 5) {
            RefreshedDigit = 0;
        }

        //set current digit
        CurrentDigit = set_digit(number, RefreshedDigit);

        //turn on next cathode
        HAL_GPIO_WritePin(GPIOE, active_cathodes[RefreshedDigit], GPIO_PIN_RESET);

        //set anodes to digit
        for (const auto i: active_segment) {
            if (common::hex_map[CurrentDigit].to_ulong() & Mask) {
                HAL_GPIO_WritePin(GPIOE, active_segment[i], GPIO_PIN_SET);
            }
            else {
                HAL_GPIO_WritePin(GPIOE, active_segment[i], GPIO_PIN_RESET);
            }
            Mask = Mask >> 1;
        }

        //possible delay, 1 or 2 ms
        //HAL_Delay(1)
    }
}
}
}