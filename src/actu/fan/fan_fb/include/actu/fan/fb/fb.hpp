#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include "actu/fan/common/common.hpp"

namespace actu {
namespace fan {
namespace fb {
    class FB {
    private:
        static const size_t ONE_OVERFLOW_DURATION_US { 27 };
        std::array<size_t, 4> recorded_overflows {};
        size_t recorded_count { 0 };
        std::optional<float> rpm { std::nullopt };
        size_t pwm_pulse_finished_overflows { 0 };
    public:
        const ::actu::fan::common::Fan& fan;

        FB(const ::actu::fan::common::Fan& fan) :
            fan{ fan }
        {}

        inline std::optional<float> consume_rpm() {
            const auto rpm_temp { rpm };
            if(rpm_temp.has_value() == false) {
                return std::nullopt;
            }

            rpm = std::nullopt;
            return rpm_temp;
        }

        inline void increment_pwm_pulse_finished_overflows() {
            pwm_pulse_finished_overflows++;
        }

        inline void record_input_capture() {
            recorded_overflows[recorded_count] = pwm_pulse_finished_overflows;
            recorded_count++;
            if(recorded_count == 4) {
                refresh_rpm();
                recorded_count = 0;
            }
        }
    private:
        inline void refresh_rpm() {
            const size_t one_rotation_total_pwm_pulse_finished_overflows {
                recorded_overflows.back()
                - recorded_overflows.front()
            };
            const size_t one_rotation_duration_us { ONE_OVERFLOW_DURATION_US * one_rotation_total_pwm_pulse_finished_overflows };
            rpm = (60'000'000.0f / static_cast<float>(one_rotation_duration_us));
        }
    };

    extern FB FAN0_REAR_LEFT;
    extern FB FAN1_FRONT_LEFT;
    extern FB FAN2_REAR_RIGHT;
    extern FB FAN3_BOTTOM_LEFT;
    extern FB FAN4_BOTTOM_RIGHT;
    extern FB FAN5_FRONT_RIGHT;
    extern const std::array<std::reference_wrapper<FB>, 6> fbs;

    HAL_StatusTypeDef init(const common::Fan& fan);
namespace all {
    HAL_StatusTypeDef init();
}
}
}
}