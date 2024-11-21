#include "main.h"
#include "sens/dht/dht.hpp"
#include "tasks/dht_test.hpp"

namespace tasks {
    DHT_Test& DHT_Test::get_instance() {
        static DHT_Test instance {};
        return instance;
    }

    void DHT_Test::worker(void* arg) {
        (void) arg;
        //DHT_Test& self { *static_cast<DHT_Test*>(arg) };
        sens::dht::Extension extension { sens::dht::Model::DHT11, DHTD_GPIO_Port, DHTD_Pin };
        while(1) {
            const auto ret { extension.get_data() };
            if(ret.has_value()) {
                std::cout
                    << "ret.value().Humidity: "
                    << ret.value().Humidity
                    << "ret.value().Temperature: "
                    << ret.value().Temperature
                    << std::endl;
            }
            osDelay(1'000);
        }
    }
}