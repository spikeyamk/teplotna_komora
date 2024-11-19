#include <trielo/trielo.hpp>
#include <cmsis_os2.h>
#include "example_submodule/public.hpp"
#include "util.hpp"
#include "tim.h"
#include "producer_consumer_test.hpp"
#include "dht/dht.hpp"

/**
 * @brief App entry point. This function cannot exit.
 * @param arg is necessary in oder for its function pointer to be of type osThreadFunc_t. Remains unused, nullptr is injected into it.
 */
extern "C" void app_main(void* arg) {
    (void) arg;
    redirect_printf();
    std::printf("Hello from app_main\r\n");

    for(size_t tick = 0; true; tick++) {
        std::printf("app_main: tick: %zu\r\n", tick);
        const auto dht { DHT_GetData() };
        std::cout
            << "dht.Temperature: "
            << dht.Temperature
            << " dht.Humidity: "
            << dht.Humidity
            << std::endl;
        osDelay(1'000);
    }

    // we should never get here
}