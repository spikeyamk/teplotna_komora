#include <trielo/trielo.hpp>
#include "inner_submodule/inner_submodule.hpp"
#include "private.hpp"

namespace submodule {
    void bar() {
        Trielo::trielo<inner_submodule::foo>();
    }
}