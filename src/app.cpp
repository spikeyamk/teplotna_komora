#include <trielo/trielo.hpp>
#include "submodule/public.hpp"
#include "returns_true.hpp"
#include "app.hpp"

/// This function calculates the area of a rectangle.
int run(int width, int height) {
    (void) width;
    (void) height;
    /* STM32H503x has 128K FLASH only these functions don't fit into it */
    //Trielo::trielo<submodule::foo>();
    //Trielo::trielo<returns_true>();
    std::printf("Hello World!\n\r");
    return 0;
}