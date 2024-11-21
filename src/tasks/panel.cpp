#include "util/util.hpp"
#include "tasks/panel.hpp"

namespace tasks {
    Panel& Panel::get_instance() {
        static Panel instance {};
        return instance;
    }

    void Panel::Menu::Actions::stop_blinking(Panel& self) {
        SevsegWhite::get_instance().blinking = false;
        TempCtl::get_instance().push(
            self.menu_properties.running.value
            ? TempCtl::Controller::Events::Variant(TempCtl::Controller::Events::TurnOn())
            : TempCtl::Controller::Events::Variant(TempCtl::Controller::Events::TurnOff())
        );

        TempCtl::get_instance().push(
            TempCtl::Controller::Events::Configuration {
                .desired_rtd = self.menu_properties.desired_rtd.value,
                .broiler = self.menu_properties.broiler.value,
                .pump = self.menu_properties.pump.value,
                .fan_max_rpm = self.menu_properties.fan_max_rpm.value,
                .algorithm = self.menu_properties.algorithm.value,
                .dac_front = self.menu_properties.dac_front.value,
                .dac_rear = self.menu_properties.dac_rear.value,
                .hbridge_front = self.menu_properties.hbridge_front.value,
                .hbridge_rear = self.menu_properties.hbridge_rear.value,
            }
        );
    }

    auto Panel::Menu::operator()() const {
		using namespace boost::sml;
        using namespace std;
        return make_transition_table(
            *state<States::Off> + event<Events::TurnOn> / Actions::show_previous_sevmaps<States::MAX31865>  = state<States::MAX31865>,

            state<States::MAX31865> + event<Events::TurnOff> / Actions::turn_off = state<States::Off>,
            state<States::MAX31865> + event<Events::MAX31865_Sample> / Actions::show_sample<States::MAX31865, Events::MAX31865_Sample> = state<States::MAX31865>,
            state<States::MAX31865> + event<Events::NextItem> / Actions::show_previous_sevmaps<States::SHT31_Inside> = state<States::SHT31_Inside>,

            state<States::SHT31_Inside> + event<Events::TurnOff> / Actions::turn_off = state<States::Off>,
            state<States::SHT31_Inside> + event<Events::SHT31_InsideSample> / Actions::show_sample<States::SHT31_Inside, Events::SHT31_InsideSample> = state<States::SHT31_Inside>,
            state<States::SHT31_Inside> + event<Events::NextItem> / Actions::show_previous_sevmaps<States::SHT31_Outside> = state<States::SHT31_Outside>,

            state<States::SHT31_Outside> + event<Events::TurnOff> / Actions::turn_off = state<States::Off>,
            state<States::SHT31_Outside> + event<Events::SHT31_OutsideSample> / Actions::show_sample<States::SHT31_Outside, Events::SHT31_OutsideSample> = state<States::SHT31_Outside>,
            state<States::SHT31_Outside> + event<Events::NextItem> / Actions::first_property = state<States::Property>,

            state<States::Property> + event<Events::TurnOff> [Guards::is_not_blinking] / Actions::turn_off = state<States::Off>,
            state<States::Property> + event<Events::NextItem> [Guards::is_last_property_and_is_not_blinking] / Actions::show_previous_sevmaps<States::MAX31865> = state<States::MAX31865>,
            state<States::Property> + event<Events::NextItem> [Guards::is_not_blinking] / Actions::next_property = state<States::Property>,

            state<States::Property> + event<Events::BlinkToggle> [Guards::is_blinking] / function(Actions::stop_blinking) = state<States::Property>,
            state<States::Property> + event<Events::BlinkToggle> / Actions::start_blinking = state<States::Property>,
            state<States::Property> + event<Events::Increment> [Guards::is_blinking] / Actions::increment = state<States::Property>,
            state<States::Property> + event<Events::Decrement> [Guards::is_blinking] / Actions::decrement = state<States::Property>,
            state<States::Property> + event<Events::NextStep> / Actions::next_step = state<States::Property>
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

        boost::sml::sm<Menu> sm { self.menu_property, self.menu_properties.properties, self.max6549, self.on_off, self.max31865, self.sht31_inside, self.sht31_outside, self };
        sm.process_event(Menu::Events::TurnOn());
 
        while(1) {
            const auto event { self.pop() };
            std::visit([&](auto&& e) {
                sm.process_event(e);
            }, event);
        }
    }

    osStatus_t Panel::push(const Menu::Events::Variant& event, const Timeout timeout) {
        return osMessageQueuePut(queue, static_cast<const void*>(&event), 0, static_cast<uint32_t>(timeout));
    }

    Panel::Menu::Events::Variant Panel::pop() {
        Menu::Events::Variant event;
        while(osMessageQueueGet(queue, reinterpret_cast<void*>(&event), nullptr, osWaitForever) != osOK) {
            osDelay(1);
        }
        return event;
    }
}