#include <trielo/trielo.hpp>
#include "example_submodule/public.hpp"
#include "app/app.hpp"

/// This function calculates the area of a rectangle.
void app_main(void* arg) {
    (void) arg;
    Trielo::trielo<example_submodule::foo>();
    while(1) {
        std::printf("Hello World!\n\r");
    }
}