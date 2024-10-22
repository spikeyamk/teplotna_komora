#include <trielo/trielo.hpp>
#include "bar.hpp"
#include "inner_submodule/inner_submodule.hpp"

namespace inner_submodule {
    void foo() {
        Trielo::trielo<bar>();
    }
}