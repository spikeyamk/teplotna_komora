#include <trielo/trielo.hpp>
#include "private.hpp"
#include "example_submodule/public.hpp"

namespace example_submodule {
    void foo() {
        Trielo::trielo<bar>();
    }
}