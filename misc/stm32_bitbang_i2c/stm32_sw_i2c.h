/*
  stm32_sw_i2c.h - Library for implementing I2C in SW on any GPIO on STM32 MCU.
  Created by Mawaba Pascal Dao, Dec 2, 2020.
  Released into the public domain.
*/
#ifndef __STM32_SW_I2C_H
#define __STM32_SW_I2C_H

#include "main.h"
#include "dwt_stm32_delay.h"
#include "cmsis_os2.h"
#include "core_cm7.h"

namespace i2c2 {
namespace stm32_bitbang_i2c {
    void init();
    void deinit();
    void scan();
}
}

#define I2C_CLEAR_SDA HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_RESET);
#define I2C_SET_SDA HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_SET);
//#define I2C_READ_SDA {if (HAL_GPIO_ReadPin(SW_I2C_SDA_GPIO_Port, SW_I2C_SDA_Pin)) == GPIO_PIN_SET) return 1; else return 0; return 0;};
#define I2C_CLEAR_SCL HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_RESET);
#define I2C_SET_SCL HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_SET);

#define SYSTICK_LOAD (SystemCoreClock/1000000U)
#define SYSTICK_DELAY_CALIB (SYSTICK_LOAD >> 1)

#define DELAY_US(us) \
    do { \
         uint32_t start = SysTick->VAL; \
         uint32_t ticks = (us * SYSTICK_LOAD)-SYSTICK_DELAY_CALIB;  \
         while((start - SysTick->VAL) < ticks); \
    } while (0)

#define I2C_DELAY DELAY_US(5); // 5 microsecond delay
//#define I2C_DELAY osDelay(1); // 5 microsecond delay

//void I2C_bus_init(uint8_t scl_pin, uint8_t sda_pin, uint8_t port);

void I2C_init(void);

void I2C_start_cond(void);

void I2C_stop_cond(void);

void I2C_write_bit(uint8_t b);

uint8_t I2C_read_SDA(void);

// Reading a bit in I2C:
uint8_t I2C_read_bit(void);

bool I2C_write_byte(uint8_t B, bool start, bool stop);

uint8_t I2C_read_byte(bool ack, bool stop);

bool I2C_send_byte(uint8_t address, uint8_t data);

uint8_t I2C_receive_byte(uint8_t address);

bool I2C_send_byte_data(uint8_t address, uint8_t reg, uint8_t data);

uint8_t I2C_receive_byte_data(uint8_t address, uint8_t reg);

bool I2C_transmit(uint8_t address, uint8_t data[], uint8_t size);

bool I2C_receive(uint8_t address, uint8_t reg[], uint8_t *data, uint8_t reg_size, uint8_t size);

#endif /*__STM32_SW_I2C_H */