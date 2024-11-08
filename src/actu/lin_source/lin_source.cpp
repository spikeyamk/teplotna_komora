#include <iostream>
#include "cmsis_os2.h"
#include "actu/lin_source/lin_source.hpp"
#include "dac.h"

namespace actu {
namespace lin_source {
    void test_dac() {
        front::start_dac();
        rear::start_dac();
        for(
            uint12_t dac_value = 0;
            true;
            dac_value = [](const uint12_t in) {
                static constexpr uint12_t inc { 2 << 9 };
                static constexpr uint12_t stopper { 2 << 11 };
                return (in + inc) > stopper ? 0 : in + inc;
            }(dac_value)
        ) {
            front::set_output(dac_value);
            rear::set_output(dac_value);
            std::printf("dac_value: %u\n", dac_value.unwrap());
            osDelay(10'000);
        }
    }
namespace front {
    void start_dac() {
        HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
    }

    void stop_dac() {
        HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
    }

    void set_output(const uint12_t value) {
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value.unwrap());
    }

    uint12_t read_output() {
        return static_cast<uint16_t>(HAL_DAC_GetValue(&hdac, DAC_CHANNEL_1));
    }
}
namespace rear {
    void start_dac() {
        HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
    }

    void stop_dac() {
        HAL_DAC_Stop(&hdac, DAC_CHANNEL_2);
    }

    void set_output(const uint12_t value) {
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, value.unwrap());
    }

    uint12_t read_output() {
        return static_cast<uint16_t>(HAL_DAC_GetValue(&hdac, DAC_CHANNEL_2));
    }
}
}
}