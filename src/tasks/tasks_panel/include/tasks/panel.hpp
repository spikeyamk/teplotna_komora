#pragma once

#include <iostream>

#include "tasks/prototype.hpp"

namespace tasks {
    class Panel : public Prototype<Panel, 4 * 1024, "panel"> {
        friend CRTP;
    public:
        float desired_temp { 20.0f };
        const float step { 1.0f };
    private:
        const float max { 85.0f };
        const float min { -40.0f };
        Panel() = default;
    public:
        static Panel& get_instance();
    private:
        static void worker(void* arg);
    public:
        void increment();
        void decrement();
    };
}