#include "util/util.hpp"
#include "tasks/senser_killer.hpp"
#include "tasks/panel.hpp"

namespace tasks {
    Panel& Panel::get_instance() {
        static Panel instance {};
        return instance;
    }

    auto Panel::Menu::operator()() const {
		using namespace boost::sml;
        return make_transition_table(
            *state<States::Off> + event<Events::TurnOn> / Actions::turn_on = state<States::MeasuredTemp>,

            state<States::MeasuredTemp> + event<Events::TurnOff> / Actions::turn_off = state<States::Off>,
            state<States::MeasuredTemp> + event<Events::Next> = state<States::Property>,

            state<States::Property> + event<Events::TurnOff> / Actions::turn_off = state<States::Off>,
            state<States::Property> + event<Events::Next> [!Guards::is_last_property] / Actions::next = state<States::Property>,
            state<States::Property> + event<Events::Next> [Guards::is_last_property] / Actions::next = state<States::MeasuredTemp>,
            state<States::Property> + event<Events::StartBlink> / Actions::start_blinking = state<States::Property>,
            state<States::Property> + event<Events::StopBlink> / Actions::stop_blinking = state<States::Property>,
            state<States::Property> + event<Events::Increment> [Guards::is_blinking] / Actions::increment = state<States::Property>,
            state<States::Property> + event<Events::Decrement> [Guards::is_blinking] / Actions::decrement = state<States::Property>
        );
    }

    bool Panel::init() {
        const osMessageQueueAttr_t queue_attr {
            .name = "panel_queue",
            .attr_bits = 0,
            .cb_mem = &queue_control_block,
            .cb_size = sizeof(queue_control_block),
            .mq_mem = events.data(),
            .mq_size = (events.size() * sizeof(decltype(events)::value_type)),
        };

        return (queue = osMessageQueueNew(events.size(), sizeof(decltype(events)::value_type), &queue_attr)) != nullptr;
    }

    void Panel::worker(void* arg) {
        Panel& self { *static_cast<Panel*>(arg) };

        boost::sml::sm<Menu> sm { self.menu_property, self.menu_properties.properties, self.max6549 };
 
        while(1) {
            const auto event { self.pop() };
            std::printf("event.index(): %zu\n", event.index());
            std::visit([&](auto&& e) {
                sm.process_event(e);
            }, event);
        }
    }

    osStatus_t Panel::push(const Menu::Events::Variant& event, const Timeout timeout) {
        std::printf("osMessageQueuePut(queue, &event, 0, static_cast<uint32_t>(timeout)): %d\n", osMessageQueuePut(queue, static_cast<const void*>(&event), 0, static_cast<uint32_t>(timeout)));
        return osOK;
    }

    Panel::Menu::Events::Variant Panel::pop() {
        Menu::Events::Variant event;
        while(osMessageQueueGet(queue, reinterpret_cast<void*>(&event), nullptr, osWaitForever) != osOK) {
            osDelay(1);
        }
        return event;
    }
}