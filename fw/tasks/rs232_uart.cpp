#include <iostream>
#include <array>
#include <functional>
#include <cassert>
#include <limits>
#include <boost/sml.hpp>

#include "usart.h"
#include "magic/magic.hpp"
#include "actu/peltier/peltier.hpp"
#include "tasks/panel.hpp"
#include "tasks/temp_ctl.hpp"
#include "tasks/senser_killer.hpp"
#include "tasks/rs232_uart.hpp"

namespace comm {
namespace rs232_uart {
    template<>
    void actions::Connect<Transmitter>::operator()(const TransmitterBase<Transmitter>& transmitter) const {
        transmitter(magic::results::Connect());
    }

    template<>
    void actions::Disconnect<Transmitter>::operator()(const TransmitterBase<Transmitter>& transmitter) const {
        transmitter(magic::results::Disconnect());
    }

    template<>
    void actions::Nop<Transmitter>::operator()(const TransmitterBase<Transmitter>& transmitter) const {
        transmitter(magic::results::Nop());
    }

    template<>
    void actions::ReadTempCtl<Transmitter>::operator()(const TransmitterBase<Transmitter>& transmitter) const {
        transmitter(magic::results::ReadTempCtl {
            .max31865_front = std::visit([](const auto& algo) {
                    return algo->front.actual_rtd.adc_code.value.unwrap();
                }, *tasks::TempCtl::get_instance().algorithm_pairs.active_pair
            ),
            .max31865_rear = std::visit([](const auto& algo) {
                    return algo->rear.actual_rtd.adc_code.value.unwrap();
                }, *tasks::TempCtl::get_instance().algorithm_pairs.active_pair
            ),
            .dac_front = static_cast<int16_t>(
                actu::peltier::hbridge::front::get_state() == actu::peltier::hbridge::State::Off
                    ? static_cast<int16_t>(0)
                    : (
                        actu::peltier::hbridge::front::get_state() == actu::peltier::hbridge::State::Heat
                            ? static_cast<int16_t>(actu::peltier::current_source::front::get_code().unwrap())
                            : -static_cast<int16_t>(actu::peltier::current_source::front::get_code().unwrap())
                    )
            ),
            .dac_rear = static_cast<int16_t>(
                actu::peltier::hbridge::rear::get_state() == actu::peltier::hbridge::State::Off
                    ? static_cast<int16_t>(0)
                    : (
                        actu::peltier::hbridge::rear::get_state() == actu::peltier::hbridge::State::Heat
                            ? static_cast<int16_t>(actu::peltier::current_source::rear::get_code().unwrap())
                            : -static_cast<int16_t>(actu::peltier::current_source::rear::get_code().unwrap())
                    )
            ),
            .sht31_inside = tasks::SenserKiller::get_instance().sht31_inside.temp_raw,
            .sht31_outside = tasks::SenserKiller::get_instance().sht31_outside.temp_raw,
            .p_front = std::visit([](const auto& algo) {
                    return algo->front.pid.p;
                }, *tasks::TempCtl::get_instance().algorithm_pairs.active_pair
            ),
            .i_front = std::visit([](const auto& algo) {
                    return algo->front.pid.i;
                }, *tasks::TempCtl::get_instance().algorithm_pairs.active_pair
            ),
            .d_front = std::visit([](const auto& algo) {
                    return algo->front.pid.d;
                }, *tasks::TempCtl::get_instance().algorithm_pairs.active_pair
            ),
            .p_rear = std::visit([](const auto& algo) {
                    return algo->rear.pid.p;
                }, *tasks::TempCtl::get_instance().algorithm_pairs.active_pair
            ),
            .i_rear = std::visit([](const auto& algo) {
                    return algo->rear.pid.i;
                }, *tasks::TempCtl::get_instance().algorithm_pairs.active_pair
            ),
            .d_rear = std::visit([](const auto& algo) {
                    return algo->rear.pid.d;
                }, *tasks::TempCtl::get_instance().algorithm_pairs.active_pair
            ),
        });
    }

    template<>
    void Receiver<Semaphore>::wait_until_not_empty() {
        HAL_UARTEx_ReceiveToIdle_IT(&huart3, rx_buf.data(), rx_buf.size());
        while(empty()) {
            static_cast<Semaphore::CRTP&>(sem).acquire();
        }
    }
}
}

namespace tasks {
    RS232_UART& RS232_UART::get_instance() {
        static RS232_UART instance {};
        return instance;
    }

    void RS232_UART::worker(void* arg) {
        RS232_UART& self { *static_cast<RS232_UART*>(arg) };

        using namespace comm::rs232_uart;
        Transmitter transmitter { &huart3 };

        boost::sml::sm<Connection<Semaphore, Transmitter>> connection {
            self.receiver,
            static_cast<Transmitter::CRTP&>(transmitter)
        };

        while(1) {
            std::visit([&](const auto& command) {
                connection.process_event(command);
            }, self.receiver.pop_for(magic::DISCONNECT_TIMEOUT).value_or(magic::commands::Disconnect()));
            HAL_UARTEx_ReceiveToIdle_IT(&huart3, self.receiver.rx_buf.data(), self.receiver.rx_buf.size());
        }
    }
}