#pragma once

#include "stm32f2xx_hal.h"

namespace sens {
namespace i2c {
namespace common {
    void scan(I2C_HandleTypeDef* hi2c1);
}
}
}