#pragma once

#include <iostream>
#include <variant>

#include "cmsis_os2.h"
#include "actu/fan/ctl/ctl.hpp"
#include "sens/max31865/rtd.hpp"
#include "util/tmp.hpp"
#include "tasks/task.hpp"

namespace tasks {
    class TempCtl : public Task<TempCtl, 2 * 1024, "temp_ctl"> {
        friend CRTP;
    private:
        StaticSemaphore_t semaphore_control_block {};
        osSemaphoreId_t semaphore { nullptr };
    private:
        TempCtl() = default;
    public:
        enum class Algorithm {
            Daniel,
            SAR,
            P,
            PI,
            PD,
            PID,
            FPCH,
            Manual,
        };

        enum class H_Bridge {
            Off,
            Heat,
            Cool
        };
    public:
        bool running { false };
        sens::max31865::RTD desired_rtd { 40.0f };
        Algorithm algorithm { Algorithm::SAR };
        bool broiler { false };
        bool pump { false };
        actu::fan::ctl::SpeedPercentage fan_max_rpm { 0 };
        bitint::ubitint<12> dac_front { 0 };
        bitint::ubitint<12> dac_rear { 0 };
        H_Bridge hbridge_front { H_Bridge::Off };
        H_Bridge hbridge_rear { H_Bridge::Off };
    public:
        static TempCtl& get_instance();
        void release_semaphore();
        void heat_full_power();
        void cool_full_power();
    private:
        void init();
        static void worker(void* arg);
    };
}