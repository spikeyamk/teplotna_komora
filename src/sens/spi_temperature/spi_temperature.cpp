#include "spi_temperature.hpp"
#include "../MAX31865/include/MAX31865.h"

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
        float readSensorU1ATemperature() {
            MAX31865_init(&max_gpio_u1a, 4);
            return MAX31865_readTemp();
        }

        /**
         * Read the temperature from the MAX31865 U3A temperature sensor
         *
         * @return float The temperature in degrees Celsius
         */
        float readSensorU3ATemperature() {
            MAX31865_init(&max_gpio_u3a, 4);
            return MAX31865_readTemp();
        }
    }
}