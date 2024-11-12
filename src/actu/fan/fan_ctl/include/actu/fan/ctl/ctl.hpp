#pragma once

#include "actu/fan/common/common.hpp"

namespace actu {
namespace fan {
namespace ctl {
    class SpeedPercentage {
    private:
        uint32_t tim_ccr_value;
        static constexpr uint32_t min { 0 };
        static constexpr uint32_t max { 99 };
    public:
        SpeedPercentage() = delete;
        explicit constexpr SpeedPercentage(const uint32_t duty_cycle_percentage_clamped_to_99) :
            tim_ccr_value {
                duty_cycle_percentage_clamped_to_99 > max ? min : (max - duty_cycle_percentage_clamped_to_99)
            }
        {}
    public:
        constexpr uint32_t unwrap() const {
            return tim_ccr_value;
        }

        constexpr bool operator==(const SpeedPercentage& other) const {
            return tim_ccr_value == other.tim_ccr_value;
        }

        constexpr bool operator!=(const SpeedPercentage& other) const {
            return tim_ccr_value != other.tim_ccr_value;
        }

        constexpr bool operator<(const SpeedPercentage& other) const {
            return tim_ccr_value > other.tim_ccr_value;
        }

        constexpr bool operator>(const SpeedPercentage& other) const {
            return tim_ccr_value < other.tim_ccr_value;
        }

        constexpr bool operator<=(const SpeedPercentage& other) const {
            return tim_ccr_value >= other.tim_ccr_value;
        }

        constexpr bool operator>=(const SpeedPercentage& other) const {
            return tim_ccr_value <= other.tim_ccr_value;
        }

        constexpr SpeedPercentage& operator++() {
            if((tim_ccr_value - 1) >= min) {
                tim_ccr_value--;
            }
            return *this;
        }

        constexpr SpeedPercentage& operator--() {
            if((tim_ccr_value + 1) <= max) {
                tim_ccr_value++;
            }
            return *this;
        }

        constexpr SpeedPercentage operator++(int) {
            const SpeedPercentage ret { *this };
            if((tim_ccr_value - 1) >= min) {
                tim_ccr_value--;
            }
            return ret;
        }

        constexpr SpeedPercentage operator--(int) {
            const SpeedPercentage ret { *this };
            if((tim_ccr_value + 1) <= max) {
                tim_ccr_value++;
            }
            return ret;
        }
    };
    HAL_StatusTypeDef init(const common::Fan& fan);
    void set_speed(const common::Fan& fan, const SpeedPercentage speed);
    SpeedPercentage get_speed(const common::Fan& fan);
    bool is_active(const common::Fan& fan);
namespace all {
    HAL_StatusTypeDef init();
    /**
     * Sets speed of all fans
     * @param speed when
     */
    void set_speed(const SpeedPercentage speed);
    void stop();
    void start_full_speed();
    void start_half_speed();
    void start_min_speed();
    void test_speed();
}
}
}
}