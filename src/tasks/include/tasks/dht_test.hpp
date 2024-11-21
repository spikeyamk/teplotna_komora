#pragma once

#include <iostream>

#include "tasks/task.hpp"

namespace tasks {
    class DHT_Test : public TaskBase<DHT_Test, 2 * 1024, "dht_test"> {
    private:
        DHT_Test() = default;
    public:
        static DHT_Test& get_instance();
        static void worker(void* arg);
    };
}