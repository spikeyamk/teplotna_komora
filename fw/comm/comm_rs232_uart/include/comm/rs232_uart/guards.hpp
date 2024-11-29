#pragma once

#include "comm/rs232_uart/receiver.hpp"

namespace comm {
namespace rs232_uart {
namespace guards {
    template<typename T>
    struct ReceiverEmpty {
        bool operator()(const Receiver<T>& receiver) const {
            return receiver.empty();
        }
    };
}
}
}