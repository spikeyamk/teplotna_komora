#pragma once

#include <iostream>

#include "tasks/prototype.hpp"

namespace tasks {
    class FanSenser : public Prototype<FanSenser, configMINIMAL_STACK_SIZE, "fan_senser"> {
        friend CRTP;
    private:
        float rpm { 0.0f };
        bool inited { false };
        FanSenser() = default;
    public:
        static FanSenser& get_instance();
    private:
        static void worker(void* arg);
    public:
        void init();
    };
}