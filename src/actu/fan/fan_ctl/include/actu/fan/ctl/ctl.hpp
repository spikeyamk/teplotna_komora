#pragma once

#include "actu/fan/common/common.hpp"

namespace actu {
namespace fan {
namespace ctl {
    class SpeedPercentage {
    private:
        uint32_t tim_compare_reg_value;
    public:
        SpeedPercentage() = delete;
        explicit constexpr SpeedPercentage(const uint32_t duty_cycle_percentage_clamped_to_99) :
            tim_compare_reg_value {
                duty_cycle_percentage_clamped_to_99 > static_cast<uint32_t>(99) ? static_cast<uint32_t>(0) : (static_cast<uint32_t>(99) - duty_cycle_percentage_clamped_to_99)
            }
        {}
    public:
        constexpr uint32_t unwrap() const {
            return tim_compare_reg_value;
        }

        constexpr bool operator==(const SpeedPercentage& other) const {
            return tim_compare_reg_value == other.tim_compare_reg_value;
        }

        constexpr bool operator!=(const SpeedPercentage& other) const {
            return tim_compare_reg_value != other.tim_compare_reg_value;
        }

        constexpr bool operator<(const SpeedPercentage& other) const {
            return tim_compare_reg_value > other.tim_compare_reg_value;
        }

        constexpr bool operator>(const SpeedPercentage& other) const {
            return tim_compare_reg_value < other.tim_compare_reg_value;
        }

        constexpr bool operator<=(const SpeedPercentage& other) const {
            return tim_compare_reg_value >= other.tim_compare_reg_value;
        }

        constexpr bool operator>=(const SpeedPercentage& other) const {
            return tim_compare_reg_value <= other.tim_compare_reg_value;
        }
    };
    HAL_StatusTypeDef init(const common::Fan& fan);
    void set_speed(const common::Fan& fan, const SpeedPercentage speed);
    SpeedPercentage get_speed(const common::Fan& fan);
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