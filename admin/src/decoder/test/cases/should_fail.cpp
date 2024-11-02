#include <trielo/trielo.hpp>
#include "decoder/test.hpp"

int main() {
    return Trielo::trielo<decoder::test::should_fail>();
}