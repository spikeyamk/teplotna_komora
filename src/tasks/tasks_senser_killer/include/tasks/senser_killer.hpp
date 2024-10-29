#pragma once

#include <array>
#include <iostream>

#include "tasks/prototype.hpp"

namespace tasks {
    class SenserKiller : public Prototype<SenserKiller, 4096, "senser_killer"> {
    private:
        float temp_front { 0.0f };
        float temp_rear { 0.0f };
        SenserKiller() = default;
    public:
        static SenserKiller& get_instance();
        static void worker(void* arg);
    };
}