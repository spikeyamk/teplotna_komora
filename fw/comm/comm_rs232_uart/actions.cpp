#include "comm/rs232_uart/actions.hpp"

namespace comm {
namespace rs232_uart {
    template<typename T>
    struct WaitUntilChannelNotEmpty {
        void operator()(T& channel) const {
            channel.wait_until_not_empty();
        }
    };
}
}