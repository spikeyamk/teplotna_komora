#pragma once

#include "stm32f2xx_hal.h"
#include "comm/rs232_uart/transmitter_base.hpp"

namespace comm {
namespace rs232_uart {
    class Transmitter : public TransmitterBase<Transmitter> {
        friend CRTP;
    private:
        UART_HandleTypeDef* huart { nullptr };
    public:
        Transmitter(UART_HandleTypeDef* huart);
    private:
        void operator()(const ResultVariant& result_variant) const;
    };
}
}