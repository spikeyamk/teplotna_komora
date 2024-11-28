#pragma once

#include "comm/rs232_uart/channel.hpp"
#include "comm/rs232_uart/transmitter_base.hpp"

namespace comm {
namespace rs232_uart {
namespace actions {
    template<typename T, typename U = void, typename V = void>
    struct WaitUntilChannelNotEmpty {
        void operator()(T& channel) const {
            channel.wait_until_not_empty();
        }
    };

    template<typename T>
    struct Connect {
        void operator()(const TransmitterBase<T>& transmitter) const;
    };

    template<typename T>
    struct Disconnect {
        void operator()(const TransmitterBase<T>& transmitter) const;
    };

    template<typename T>
    struct Nop {
        void operator()(const TransmitterBase<T>& transmitter) const;
    };

    template<typename T>
    struct ReadTempCtl {
        void operator()(const TransmitterBase<T>& transmitter) const;
    };
}
}
}