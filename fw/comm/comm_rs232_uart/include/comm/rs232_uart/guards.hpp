#pragma once

#include "comm/rs232_uart/channel.hpp"

namespace comm {
namespace rs232_uart {
namespace guards {
    template<typename T>
    struct ChannelEmpty {
        bool operator()(T& channel) const {
            return channel.empty();
        }
    };
}
}
}