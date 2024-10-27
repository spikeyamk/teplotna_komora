#pragma once

#include <iostream>

#include "tasks/prototype.hpp"

namespace tasks {
    class Panel : public Prototype<Panel, configMINIMAL_STACK_SIZE, "panel"> {
    private:
        float rpm { 0.0f };
        Panel() = default;
    public:
        static Panel& get_instance();
        static void worker(void* arg);
    };
}