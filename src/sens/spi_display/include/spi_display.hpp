#ifndef SPI_DISPLAY_HPP
#define SPI_DISPLAY_HPP

#include "../MAX6954/include/MAX6954.h"

namespace sens {
    namespace spi_display {
        /**
         * Write a float number to the MAX6954 display
         *
         * @param number The number to write
         * @param color The color of the display (YELLOW or GREEN)
         */
        void writeFloatToDisplay(float number, enum DisplayColor color);
    }
}

#endif // SPI_DISPLAY_HPP