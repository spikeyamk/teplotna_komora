#include <trielo/trielo.hpp>
#include "comm/rs232_uart/test.hpp"

int main() {
    return Trielo::trielo<comm::rs232_uart::test::expected_sequence>();
}