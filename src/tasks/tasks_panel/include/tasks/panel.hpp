#pragma once

#include <iostream>

#include "tasks/prototype.hpp"

namespace tasks {
    class Panel : public Prototype<Panel, 4096, "panel"> {
    public:
        float number { 20.0f };
        const float step { 1.0f };
    private:
        const float max { 85.0f };
        const float min { -20.0f };
        Panel() = default;
    public:
        static Panel& get_instance();
        static void worker(void* arg);
        void increment();
        void decrement();
    };
}