#include <trielo/trielo.hpp>
#include <cmsis_os2.h>
#include "example_submodule/public.hpp"
#include "util.hpp"

/**
 * @brief App entry point. This function cannot exit.
 * @param arg is necessary in oder for its function pointer to be of type osThreadFunc_t. Remains unused, nullptr is injected into it.
 */
extern "C" void app_main(void* arg) {
    (void) arg;
    redirect_printf();
    Trielo::trielo<example_submodule::foo>();

    for(size_t tick = 0; true; tick++) {
        if(tick % 1'000 == 0)
            std::printf("app_main: tick: %zu\r\n", tick);
        osDelay(1);
    }

    // we should never get here
}