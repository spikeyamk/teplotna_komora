#pragma once

#include <iostream>

#include "tasks/prototype.hpp"

namespace tasks {
    class TempCtl : public Prototype<TempCtl, 4 * 1024, "temp_ctl"> {
        friend CRTP;
    private:
        float rpm { 0.0f };
        TempCtl() = default;
    public:
        static TempCtl& get_instance();
    private:
        static void worker(void* arg);
    };
}