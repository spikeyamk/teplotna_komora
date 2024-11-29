#include <trielo/trielo.hpp>
#include "private.hpp"
#include "example_subdirectory/public.hpp"

namespace example_subdirectory {
    void foo() {
        Trielo::trielo<bar>();
    }
}