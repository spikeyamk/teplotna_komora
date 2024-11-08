#include <trielo/trielo.hpp>
#include "sens/max31865/test.hpp"

int main() {
    return Trielo::trielo<sens::max31865::test::rtd>();
}