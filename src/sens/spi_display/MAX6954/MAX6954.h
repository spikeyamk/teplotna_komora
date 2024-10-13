/**
 * @file MAX6954.h
 * @author Michael Murithi (xkariuki@stuba.sk)
 * @brief This library uses the MAX6954 LED driver to control the LED display.
 * This library is written for Stm32f2 microcontrollers using the stm32-HAL software library.
 * The library uses the GPIO pins of the microcontroller to emulate the SPI communication.
 * The configurations are based on the official MAX6954 datasheet.
 * @version 0.1
 * @date 2024-10-13
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MAX6954_LIB_MAX6954_H
#define MAX6954_LIB_MAX6954_H

#include <stm32f205xx.h>
#include "stm32f2xx_hal.h"
#include <stdbool.h>

/********************* MAX6954 registers and config bits *********************/
#define DELAY(x) HAL_Delay(x) // Adjust for your delay function

// MAX6954 configuration registers
#define MAX6954_WRITE 0x00          // 0 for write
#define MAX6954_CONFIG_REG 0x00     // Address for the configuration register
#define MAX6954_BRIGHTNESS_REG 0x01 // Address for the brightness register

// Configurations for the yellow 7-segment display with 5 digits (DS4, DS7, DS10, DS12, DS14)
#define MAX6954_DIGIT_ADDRESS_DS4 0x20
#define MAX6954_DIGIT_ADDRESS_DS7 0x28
#define MAX6954_DIGIT_ADDRESS_DS10 0x21
#define MAX6954_DIGIT_ADDRESS_DS12 0x29
#define MAX6954_DIGIT_ADDRESS_DS14 0x22

// Configurations for the green 7-segment display with 5 digits (DS5, DS8, DS11, DS13, DS15)
#define MAX6954_DIGIT_ADDRESS_DS5 0x24
#define MAX6954_DIGIT_ADDRESS_DS8 0x2C
#define MAX6954_DIGIT_ADDRESS_DS11 0x25
#define MAX6954_DIGIT_ADDRESS_DS13 0x2D
#define MAX6954_DIGIT_ADDRESS_DS15 0x26

// SPI pin configuration for MAX6954
#define MAX6954_CE_PORT GPIOB
#define MAX6954_CE_PIN GPIO_PIN_12

#define MAX6954_CLK_PORT GPIOB
#define MAX6954_CLK_PIN GPIO_PIN_13

#define MAX6954_MOSI_PORT GPIOB
#define MAX6954_MOSI_PIN GPIO_PIN_15

#define MAX6954_MISO_PORT GPIOB
#define MAX6954_MISO_PIN GPIO_PIN_14

typedef struct
{
    int digits[5];
    bool decimal_points[5];
} MAX6954_Float_Digits;

enum DisplayColor
{
    YELLOW,
    GREEN
};

/********************* Public functions *********************/
void MAX6954_init();
void MAX6954_write(uint8_t address, uint8_t data);

#endif // MAX6954_LIB_MAX6954_H