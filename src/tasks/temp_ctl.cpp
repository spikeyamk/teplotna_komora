#include <functional>
#include <cassert>
#include <trielo/trielo.hpp>
#include "actu/fan/broiler/broiler.hpp"
#include "actu/peltier/peltier.hpp"
#include "actu/fan/broiler/broiler.hpp"
#include "actu/pump/pump.hpp"
#include "util/util.hpp"
#include "tasks/temp_ctl.hpp"

namespace tasks {
    TempCtl& TempCtl::get_instance() {
        static TempCtl instance {};
        return instance;
    }

    void TempCtl::Controller::Actions::turn_off(TempCtl& self) {
        util::turn_every_annoying_peripheral_off();
        std::visit([](auto&& algo) {
            algo->front.pid.reset();
            algo->rear.pid.reset();
        }, *self.algorithm_pairs.active_pair);
    }

    void TempCtl::Controller::Actions::save_configuration(TempCtl& self, const Events::Configuration& event) {
        self.configuration = event;
    }

    template<>
    void TempCtl::Controller::Actions::save_sample<TempCtl::Controller::Events::MAX31865_Sample>(TempCtl& self, const TempCtl::Controller::Events::MAX31865_Sample& event) {
        self.max31865_sample = event;
    }

    template<>
    void TempCtl::Controller::Actions::save_sample<TempCtl::Controller::Events::SHT31_Sample>(TempCtl& self, const TempCtl::Controller::Events::SHT31_Sample& event) {
        self.sht31_sample = event;
    }
    
    void TempCtl::Controller::Actions::run_algorithm(TempCtl& self, const TempCtl::Controller::Events::MAX31865_Sample& event) {
        save_sample(self, event);
        self.apply_configuration();
        std::visit([&](auto&& algo) {
            algo->run();
        }, *self.algorithm_pairs.active_pair);
    };

    TempCtl::TempCtl() {
        actu::fan::ctl::all::init();

        actu::peltier::current_source::front::init();
        actu::peltier::current_source::rear::init();

        util::turn_every_annoying_peripheral_off();

        const osMessageQueueAttr_t queue_attr {
            .name = "panel_queue",
            .attr_bits = 0,
            .cb_mem = &queue_control_block,
            .cb_size = sizeof(queue_control_block),
            .mq_mem = events.data(),
            .mq_size = (events.size() * sizeof(decltype(events)::value_type)),
        };

        queue = osMessageQueueNew(events.size(), sizeof(decltype(events)::value_type), &queue_attr);
        assert(queue != nullptr);
    }

    auto TempCtl::Controller::operator()() const {
		using namespace boost::sml;
        using namespace std;
        return make_transition_table(
            *state<States::Off> + event<Events::TurnOn> = state<States::On>,

            state<States::On> + event<Events::TurnOff> / function(Actions::turn_off) = state<States::Off>,
            state<States::On> + event<Events::Configuration> / function(Actions::save_configuration) = state<States::On>,
            state<States::On> + event<Events::SHT31_Sample> / function(Actions::save_sample<Events::SHT31_Sample>) = state<States::On>,
            state<States::On> + event<Events::MAX31865_Sample> / function(Actions::run_algorithm) = state<States::On>
        );
    }

    void TempCtl::worker(void* arg) {
        TempCtl& self { *static_cast<TempCtl*>(arg) };
        boost::sml::sm<Controller> sm { self };

        while(1) {
            const auto event { self.pop() };
            std::visit([&](auto&& e) {
                sm.process_event(e);
            }, event);
        }
    }

    osStatus_t TempCtl::push(const Controller::Events::Variant& event) {
        return osMessageQueuePut(queue, static_cast<const void*>(&event), 0, 30);
    }

    TempCtl::Controller::Events::Variant TempCtl::pop() {
        TempCtl::Controller::Events::Variant event;
        while(osMessageQueueGet(queue, reinterpret_cast<void*>(&event), nullptr, osWaitForever) != osOK) {
            osDelay(1);
        }
        return event;
    }

    void TempCtl::apply_configuration() {
        if(configuration.broiler) {
            actu::fan::broiler::start();
        } else {
            actu::fan::broiler::stop();
        }

        if(configuration.pump) {
            actu::pump::start();
        } else {
            actu::pump::stop();
        }

        actu::fan::ctl::all::set_speed(configuration.fan_max_rpm);

        algorithm_pairs.active_pair = algorithm_pairs.array.begin() + static_cast<size_t>(configuration.algorithm);
    }
}