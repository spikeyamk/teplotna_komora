#pragma once

#include <iostream>

#include "tasks/prototype.hpp"

namespace tasks {
    class TempCtl : public Prototype<TempCtl, 2 * 1024, "temp_ctl"> {
        friend CRTP;
    private:
        TempCtl() = default;
    public:
        static TempCtl& get_instance();
    private:
        void init();
        static void worker(void* arg);
    };
}