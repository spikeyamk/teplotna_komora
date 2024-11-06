#pragma once

#include <iostream>

#include "tasks/prototype.hpp"

namespace tasks {
    class Example : public Prototype<Example, configMINIMAL_STACK_SIZE, "example"> {
    private:
        float rpm { 0.0f };
        Example() = default;
    public:
        static Example& get_instance();
        static void worker(void* arg);
    };
}