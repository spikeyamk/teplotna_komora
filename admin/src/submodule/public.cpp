#include <trielo/trielo.hpp>
#include "private.hpp"
#include "submodule/public.hpp"

namespace submodule {
    void foo() {
        Trielo::trielo<bar>();
    }
}