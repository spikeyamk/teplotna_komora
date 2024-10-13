#include "spi_temperature.hpp"
#include "MAX31865.h"

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