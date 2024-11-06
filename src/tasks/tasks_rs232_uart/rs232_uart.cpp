#include <iostream>
#include <array>
#include <functional>
#include <boost/sml.hpp>

#include "tasks/temp_senser.hpp"
#include "tasks/panel.hpp"
#include "tasks/rs232_uart.hpp"

namespace tasks {
    RS232_UART& RS232_UART::get_instance() {
        static RS232_UART instance {};
        return instance;
    }

    auto RS232_UART::Connection::operator()() const {
		using namespace boost::sml;
		using namespace std;
        using namespace common::magic;
		return make_transition_table(
			*state<States::Disconnected> + event<commands::Connect> / function{Actions::connect} = state<States::Connected>,
            state<States::Connected> + event<commands::Disconnect> / function{Actions::disconnect} = state<States::Disconnected>,

            state<States::Connected> + event<commands::Nop> / function{Actions::nop} = state<States::Connected>,
            state<States::Connected> + event<commands::ReadSensors> / function{Actions::read_sensors} = state<States::Connected>,
            state<States::Connected> + event<commands::WriteTemp> / function{Actions::write_temp} = state<States::Connected>
		);
    }

    void RS232_UART::Connection::Actions::connect(RS232_UART& self) {
        self.transmit(common::magic::results::Connect {});
    }

    void RS232_UART::Connection::Actions::disconnect(RS232_UART& self) {
        self.transmit(common::magic::results::Disconnect {});
    }

    void RS232_UART::Connection::Actions::nop(RS232_UART& self) {
        self.transmit(common::magic::results::Nop {});
    }

    void RS232_UART::Connection::Actions::read_sensors(const RS232_UART& self) {
        self.transmit(
            common::magic::results::ReadSensors {
                .temp_front = TempSenser::get_instance().temp_front,
                .temp_rear = TempSenser::get_instance().temp_rear,
            }
        );
    }

    void RS232_UART::Connection::Actions::write_temp(RS232_UART& self, const common::magic::commands::WriteTemp& write_temp) {
        Panel::get_instance().desired_temp = write_temp.value;
        self.transmit(
            common::magic::results::WriteTemp {
                .value = Panel::get_instance().desired_temp,
            }
        );
    }
    
    void RS232_UART::worker(void* arg) {
        RS232_UART& self { *static_cast<RS232_UART*>(arg) };

        using namespace common::magic;

        std::array<uint8_t, MTU> buf {};
        uint16_t rx_len { 0 };
        boost::sml::sm<Connection> sm { self };

        while(1) {
            if(HAL_UARTEx_ReceiveToIdle(&huart3, buf.data(), buf.size(), &rx_len, TIMEOUT_MS) != HAL_OK) {
                sm.process_event(commands::Disconnect());
                osDelay(1);
                continue;
            }

            const auto decoded { commands::Deserializer::decode(buf.begin(), buf.begin() + rx_len) };
            if(decoded.has_value() == false) {
                sm.process_event(commands::Disconnect());
                osDelay(1);
                continue;
            }

            std::visit([&sm](auto&& command) {
                sm.process_event(command);
            }, decoded.value());

            osDelay(1);
        }
    }
}