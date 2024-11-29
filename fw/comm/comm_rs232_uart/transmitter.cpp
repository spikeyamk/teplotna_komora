#include <cassert>
#include "magic/results/serializer.hpp"
#include "comm/rs232_uart/transmitter.hpp"

namespace comm {
namespace rs232_uart {
    Transmitter::Transmitter(UART_HandleTypeDef* huart) :
        huart { huart }
    {
        assert(huart != nullptr);
    }

    void Transmitter::operator()(const ResultVariant& result_variant) const {
        std::visit([&](const auto& result) {
            const auto result_serialized { magic::results::Serializer::run(result) };
            HAL_UART_Transmit(huart, result_serialized.data(), result_serialized.size(), HAL_MAX_DELAY);
        }, result_variant);
    }
}
}