#pragma once

#include <iostream>

#include "tasks/prototype.hpp"

namespace tasks {
    class TempCtl : public Prototype<TempCtl, configMINIMAL_STACK_SIZE, "temp_ctl"> {
    private:
        float rpm { 0.0f };
        TempCtl() = default;
    public:
        static TempCtl& get_instance();
        static void worker(void* arg);
    };
}