#include <limits>
#include <trielo/trielo.hpp>
#include "cmsis_os2.h"
#include "tim.h"
#include "actu/fan/ctl/ctl.hpp"

namespace actu {
namespace fan {
namespace ctl {
    HAL_StatusTypeDef init(const common::Fan& fan) {
        return HAL_TIM_PWM_Start(fan.htim, fan.ctl_channel.mask_for_init);
    }

    void set_speed(const common::Fan& fan, const SpeedPercentage speed) {
        __HAL_TIM_SET_COMPARE(fan.htim, fan.ctl_channel.mask_for_init, speed.unwrap_less_is_more());
    }
    
    SpeedPercentage get_speed(const common::Fan& fan) {
        return SpeedPercentage(static_cast<uint8_t>(__HAL_TIM_GET_COMPARE(fan.htim, fan.ctl_channel.mask_for_init)));
    }

    bool is_active(const common::Fan& fan) {
        if(get_speed(fan) < SpeedPercentage(4)) {
            return false;
        }

        return true;
    }
namespace all {
    HAL_StatusTypeDef init() {
        for(const auto& e: common::fans) {
            const HAL_StatusTypeDef err_ret { ::actu::fan::ctl::init(e) };
            if(err_ret != HAL_OK) {
                return err_ret;
            }
        }
        return HAL_OK;
    }

    void set_speed(const SpeedPercentage speed) {
        for(const auto& e: common::fans) {
            ::actu::fan::ctl::set_speed(e, speed);
        }
    }

    void stop() {
        ::actu::fan::ctl::all::set_speed(SpeedPercentage(0));
    }

    void start_full_speed() {
        ::actu::fan::ctl::all::set_speed(SpeedPercentage(99));
    }

    void start_half_speed() {
        ::actu::fan::ctl::all::set_speed(SpeedPercentage(50));
    }

    void start_min_speed() {
        ::actu::fan::ctl::all::set_speed(SpeedPercentage(5));
    }

    void test_speed() {
        for(uint8_t i = 0; true; i = (i > 99 ? 0 : (i + 10))) {
            Trielo::trielo<::actu::fan::ctl::all::set_speed>(SpeedPercentage(i));
            osDelay(5'000);
        }
    }
}
}
}
}