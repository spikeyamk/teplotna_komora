#ifndef SPI_TEMPERATURE_H
#define SPI_TEMPERATURE_H

#include <stm32f205xx.h>
#include "stm32f2xx_hal.h"
#include "../MAX31865/include/MAX31865.h"

// SPI pin configuration for MAX31865 U1A temperature sensor
#define MAX31865_CE_PORT_U1A GPIOD
#define MAX31865_CE_PIN_U1A GPIO_PIN_5

#define MAX31865_CLK_PORT_U1A GPIOB
#define MAX31865_CLK_PIN_U1A GPIO_PIN_3

#define MAX31865_MOSI_PORT_U1A GPIOB
#define MAX31865_MOSI_PIN_U1A GPIO_PIN_5

#define MAX31865_MISO_PORT_U1A GPIOB
#define MAX31865_MISO_PIN_U1A GPIO_PIN_4

// SPI pin configuration for MAX31865 U3A temperature sensor
#define MAX31865_CE_PORT_U3A GPIOD
#define MAX31865_CE_PIN_U3A GPIO_PIN_7

#define MAX31865_CLK_PORT_U3A GPIOB
#define MAX31865_CLK_PIN_U3A GPIO_PIN_3

#define MAX31865_MOSI_PORT_U3A GPIOB
#define MAX31865_MOSI_PIN_U3A GPIO_PIN_5

#define MAX31865_MISO_PORT_U3A GPIOB
#define MAX31865_MISO_PIN_U3A GPIO_PIN_4

// Configurations to use while intializing the MAX31865 U1A temperature sensor
MAX31865_GPIO max_gpio_u1a = {
    MAX31865_CE_PORT_U1A,
    MAX31865_CE_PIN_U1A,
    MAX31865_CLK_PORT_U1A,
    MAX31865_CLK_PIN_U1A,
    MAX31865_MOSI_PORT_U1A,
    MAX31865_MOSI_PIN_U1A,
    MAX31865_MISO_PORT_U1A,
    MAX31865_MISO_PIN_U1A
};

// Configurations to use while intializing the MAX31865 U3A temperature sensor
MAX31865_GPIO max_gpio_u3a = {
    MAX31865_CE_PORT_U3A,
    MAX31865_CE_PIN_U3A,
    MAX31865_CLK_PORT_U3A,
    MAX31865_CLK_PIN_U3A,
    MAX31865_MOSI_PORT_U3A,
    MAX31865_MOSI_PIN_U3A,
    MAX31865_MISO_PORT_U3A,
    MAX31865_MISO_PIN_U3A
};

namespace sens {
    namespace spi_temperature {
        /**
         * Read the temperature from the MAX31865 U1A temperature sensor
         *
         * @return float The temperature in degrees Celsius
         */
        float readSensorU1ATemperature();

        /**
         * Read the temperature from the MAX31865 U3A temperature sensor
         *
         * @return float The temperature in degrees Celsius
         */
        float readSensorU3ATemperature();
    }
}
#endif //SPI_TEMPERATURE_H