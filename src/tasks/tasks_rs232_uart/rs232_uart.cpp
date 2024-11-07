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
			*"Disconnected"_s + event<commands::Connect> / function{Actions::connect} = "Connected"_s,
            "Connected"_s + event<commands::Disconnect> / function{Actions::disconnect} = "Disconnected"_s,

            "Connected"_s + event<commands::Nop> / function{Actions::nop} = "Connected"_s,
            "Connected"_s + event<commands::ReadSensors> / function{Actions::read_sensors} = "Connected"_s,
            "Connected"_s + event<commands::WriteTemp> / function{Actions::write_temp} = "Connected"_s
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
                .temp_front = TempSenser::get_instance().rtd_front.calculate_approx_temp().value(),
                .temp_rear = TempSenser::get_instance().rtd_rear.calculate_approx_temp().value(),
            }
        );
    }

    void RS232_UART::Connection::Actions::write_temp(RS232_UART& self, const common::magic::commands::WriteTemp& write_temp) {
        Panel::get_instance().desired_rtd = sens::max31865::RTD(write_temp.value);
        self.transmit(
            common::magic::results::WriteTemp {
                .value = write_temp.value,
            }
        );
    }
    
    void RS232_UART::worker(void* arg) {
        RS232_UART& self { *static_cast<RS232_UART*>(arg) };

        using namespace common::magic;

        std::array<uint8_t, MTU> buf {};
        boost::sml::sm<Connection> sm { self };

        while(1) {
            HAL_UARTEx_ReceiveToIdle_IT(&huart3, buf.data(), buf.size());
            if(osSemaphoreAcquire(self.semaphore, osWaitForever) != osOK) {
                // we should never get here because of osWaitForever...
                //std::printf("osSemaphoreAcquire(self.semaphore, osWaitForever) != osOK\n");
                continue;
            }

            /*
            for(size_t i = 0; i < self.rx_len; i++) {
                std::printf("first: buf[%zu]: 0x%02X\n", i, buf[i]);
            }
            */

            const auto first_decoded { commands::Deserializer::decode(buf.begin(), buf.begin() + self.rx_len) };
            if(first_decoded.has_value() == false) {
                //std::printf("first_decoded.has_value() == false\n");
                sm.process_event(commands::Disconnect());
                continue;
            }
            std::visit([&sm](auto&& command) {
                sm.process_event(command);
            }, first_decoded.value());

            using namespace boost::sml;
            if(sm.is("Disconnected"_s)) {
                //std::printf("sm.is(\"Disconnected\"_s)\n");
                continue;
            }

            while(1) {
                HAL_UARTEx_ReceiveToIdle_IT(&huart3, buf.data(), buf.size());
                if(osSemaphoreAcquire(self.semaphore, self.semaphore_timeout) != osOK) {
                    //std::printf("osSemaphoreAcquire(self.semaphore, self.semaphore_timeout) != osOK)\n");
                    sm.process_event(commands::Disconnect());
                    break;
                }

                /*
                for(size_t i = 0; i < self.rx_len; i++) {
                    std::printf("second: buf[%zu]: 0x%02X\n", i, buf[i]);
                }
                */

                const auto second_decoded { commands::Deserializer::decode(buf.begin(), buf.begin() + self.rx_len) };
                if(second_decoded.has_value() == false) {
                    //std::printf("second_decoded.has_value() == false\n");
                    sm.process_event(commands::Disconnect());
                    break;
                }
                std::visit([&sm](auto&& command) {
                    sm.process_event(command);
                }, second_decoded.value());

                if(sm.is("Disconnected"_s)) {
                    //std::printf("sm.is(\"Disconnected\"_s)\n");
                    break;
                }
            }

            //std::printf("bottom\n");
        }
    }

    bool RS232_UART::init() {
        const osSemaphoreAttr_t sempahore_attr {
            .name = "rs232_sem",
            .attr_bits = 0,
            .cb_mem = &semaphore_control_block,
            .cb_size = sizeof(semaphore_control_block),
        };
        
        semaphore = osSemaphoreNew(1, 0, &sempahore_attr);
        if(semaphore == nullptr) {
            return false;
        }

        return true;
    }

    osStatus RS232_UART::release_semaphore(const uint16_t in_rx_len) {
        rx_len = in_rx_len;
        return osSemaphoreRelease(semaphore);
    }
}