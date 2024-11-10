#pragma once

#include <iostream>

#include "cmsis_os2.h"
#include "actu/lin_source/lin_source.hpp"
#include "tasks/prototype.hpp"

namespace tasks {
    class TempCtl : public Prototype<TempCtl, 2 * 1024, "temp_ctl"> {
        friend CRTP;
    private:
        StaticSemaphore_t semaphore_control_block {};
        osSemaphoreId_t semaphore { nullptr };
    public:
        actu::lin_source::uint12_t dac_front { 0 };
        actu::lin_source::uint12_t dac_rear { 0 };
    private:
        TempCtl() = default;
    public:
        static TempCtl& get_instance();
        void release_semaphore();
        void heat_full_power();
        void cool_full_power();
    private:
        void init();
        static void worker(void* arg);
        void set_dac_front(const actu::lin_source::uint12_t value);
        void set_dac_rear(const actu::lin_source::uint12_t value);
    };
}