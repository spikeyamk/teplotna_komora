#pragma once

#include "comm/rs232_uart/channel.hpp"

namespace comm {
namespace rs232_uart {
namespace actions {
    template<typename T>
    struct WaitUntilChannelNotEmpty {
        void operator()(T& channel) const {
            std::cout << "WaitUntilChannelNotEmpty started\n";
            channel.wait_until_not_empty();
            std::cout << "WaitUntilChannelNotEmpty finished\n";
        }
    };

    struct Connect {
        void operator()() const;
    };

    struct Disconnect {
        void operator()() const;
    };

    struct Nop {
        void operator()() const;
    };

    struct ReadTempCtl {
        void operator()() const;
    };
}
}
}