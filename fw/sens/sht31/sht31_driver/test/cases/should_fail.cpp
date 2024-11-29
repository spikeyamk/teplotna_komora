#include <trielo/trielo.hpp>
#include "example_subdirectory/test.hpp"

int main() {
    return Trielo::trielo<example_subdirectory::test::should_fail>();
}