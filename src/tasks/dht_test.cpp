#include "sens/dht/dht.hpp"
#include "tasks/dht_test.hpp"

namespace tasks {
    DHT_Test& DHT_Test::get_instance() {
        static DHT_Test instance {};
        return instance;
    }

    void DHT_Test::worker(void* arg) {
        DHT_Test& self { *static_cast<DHT_Test*>(arg) };
        while(1) {
            //std::printf("tasks::DHT_Test::worker: self.rpm: %f\n", self.rpm);
            
            osDelay(1'000);
        }
    }
}