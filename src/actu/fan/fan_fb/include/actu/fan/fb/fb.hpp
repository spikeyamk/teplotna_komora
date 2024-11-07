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
        static const uint32_t ONE_OVERFLOW_DURATION_US { 27 };
        std::array<uint32_t, 4> recorded_overflows {};
        uint32_t recorded_count { 0 };
        uint32_t pwm_pulse_finished_overflows { 0 };
        std::optional<uint32_t> one_rotation_total_pwm_pulse_finished_overflows { std::nullopt };
    public:
        const ::actu::fan::common::Fan& fan;

        FB(const ::actu::fan::common::Fan& fan) :
            fan{ fan }
        {}

        inline std::optional<float> consume_rpm() {
            const auto one_rotation_total_pwm_pulse_finished_overflows_consumed { consume_one_rotation_total_pwm_pulse_finished_overflows() };
            if(one_rotation_total_pwm_pulse_finished_overflows_consumed.has_value() == false) {
                return std::nullopt;
            }

            return (
                60'000'000.0f
                / static_cast<float>(
                    ONE_OVERFLOW_DURATION_US
                    * one_rotation_total_pwm_pulse_finished_overflows_consumed.value()
                )
            );
        }

        inline std::optional<uint32_t> consume_one_rotation_total_pwm_pulse_finished_overflows() {
            const auto ret { one_rotation_total_pwm_pulse_finished_overflows };
            one_rotation_total_pwm_pulse_finished_overflows = std::nullopt;
            return ret;
        }

        inline void increment_pwm_pulse_finished_overflows() {
            pwm_pulse_finished_overflows++;
        }

        inline void record_input_capture() {
            recorded_overflows[recorded_count] = pwm_pulse_finished_overflows;
            recorded_count++;
            if(recorded_count == 4) {
                one_rotation_total_pwm_pulse_finished_overflows = (recorded_overflows.back() - recorded_overflows.front());
                recorded_count = 0;
            }
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

    void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
    void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);

namespace all {
    HAL_StatusTypeDef init();
    void test();
}
}
}
}