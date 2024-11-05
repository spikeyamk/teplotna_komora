#pragma once

#include <iostream>

#include "tasks/prototype.hpp"

namespace tasks {
    class Example : public Prototype<Example, 4 * 1024, "example"> {
        friend CRTP;
    private:
        float rpm { 0.0f };
        Example() = default;
    public:
        static Example& get_instance();
    private:
        static void worker(void* arg);
    };
}