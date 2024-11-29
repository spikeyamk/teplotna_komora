#pragma once

#include <boost/sml.hpp>

#include "magic/commands/commands.hpp"
#include "comm/rs232_uart/guards.hpp"
#include "comm/rs232_uart/actions.hpp"

namespace comm {
namespace rs232_uart {
namespace states {
    struct Idle {};
    struct Disconnected {};
    struct Connected {};
}
    /*
     * @brief Represents the connection with states and transitions.
     *
     * This class implements a state machine for handling connections,
     * including idle, connected, and disconnected states. Actions and guards are
     * defined for transitions between these states.
     * 
    */
    template<typename T, typename U>
    class Connection {
    public:
        using Semaphore = T;
        using Transmitter = U;

        Connection() = default;
        auto operator()() const {
            using namespace boost::sml;
            using namespace magic;
            return make_transition_table(
                *state<states::Idle> / actions::WaitUntilReceiverNotEmpty<Semaphore>() = state<states::Disconnected>,

                state<states::Disconnected> [guards::ReceiverEmpty<Semaphore>()] = state<states::Idle>,
                state<states::Disconnected> + event<commands::Connect> / actions::Connect<Transmitter>() = state<states::Connected>,

                state<states::Connected> + event<commands::Disconnect> / actions::Disconnect<Transmitter>() = state<states::Disconnected>,
                state<states::Connected> + event<commands::Connect> / actions::Disconnect<Transmitter>() = state<states::Disconnected>,
                state<states::Connected> + event<commands::Nop> / actions::Nop<Transmitter>() = state<states::Connected>,
                state<states::Connected> + event<commands::ReadTempCtl> / actions::ReadTempCtl<Transmitter>() = state<states::Connected>
            );
        }
    };
}
}