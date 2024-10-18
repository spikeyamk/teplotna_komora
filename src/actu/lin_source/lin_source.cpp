#include <iostream>
#include "actu/lin_source/lin_source.hpp"
#include "dac.h"

namespace actu {
namespace lin_source {
    void start_dac() {
        HAL_DAC_Start(&hdac, DAC_CHANNEL_1);  // Start DAC on PA4
        HAL_DAC_Start(&hdac, DAC_CHANNEL_2);  // Start DAC on PA5
    }

    void set_output(uint32_t val_a, uint32_t val_b) {
        // Write value to DAC channel 2 (PA5)
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, val_a);

        // Write value to DAC channel 1 (PA4)
        HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, val_b);
    }

    void test_dac() {
        start_dac();
        for(
            uint16_t dac_value = 0;
            true;
            dac_value = [](const uint16_t in) {
                static constexpr uint16_t inc { 2 << 9 };
                static constexpr uint16_t stopper { 2 << 11 };
                return (in + inc) > stopper ? 0 : in + inc;
            }(dac_value)
        ) {
            HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value);
            HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dac_value);
            std::printf("dac_value: %u\n", dac_value);
            HAL_Delay(10'000);
        }
    }
}
}