#include <iostream>
#include "cmsis_os2.h"
#include "dac.h"
#include "actu/peltier/current_source.hpp"

namespace actu {
namespace peltier {
namespace current_source {
    void test() {
        front::init();
        rear::init();
        for(
            uint12_t dac_value = 0;
            true;
            dac_value = [](const uint12_t in) {
                static constexpr uint12_t inc { 2 << 9 };
                static constexpr uint12_t stopper { 2 << 11 };
                return (in + inc) > stopper ? 0 : in + inc;
            }(dac_value)
        ) {
            front::set(dac_value);
            rear::set(dac_value);
            std::printf("dac_value: %u\n", dac_value.unwrap());
            osDelay(10'000);
        }
    }
namespace front {
    void init() {
        HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
    }

    void set(const uint12_t code) {
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, code.unwrap());
    }

    uint12_t get_code() {
        return static_cast<uint16_t>(HAL_DAC_GetValue(&hdac, DAC_CHANNEL_1));
    }
}
namespace rear {
    void init() {
        HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
    }

    void set(const uint12_t code) {
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, code.unwrap());
    }

    uint12_t get_code() {
        return static_cast<uint16_t>(HAL_DAC_GetValue(&hdac, DAC_CHANNEL_2));
    }
}
}
}
}