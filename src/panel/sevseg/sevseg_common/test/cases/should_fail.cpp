#include <trielo/trielo.hpp>
#include "panel/sevseg/common/test.hpp"

int main() {
    return Trielo::trielo<panel::sevseg::common::test::should_fail>();
}