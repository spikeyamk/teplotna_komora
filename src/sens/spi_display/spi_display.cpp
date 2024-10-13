#include "spi_display.hpp"

namespace sens {
    namespace spi_display {
        void writeFloatToDisplay(float number, enum DisplayColor color) {
            MAX6954_init();
            MAX6954_display_float(number, color);
        }
    }
}