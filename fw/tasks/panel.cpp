#include <cassert>

#include "util/util.hpp"
#include "tasks/panel.hpp"

namespace tasks {
    Panel& Panel::get_instance() {
        static Panel instance {};
        return instance;
    }
}

namespace tasks {
    void Panel::Menu::Actions::stop_blinking(Panel& self) {
        SevsegWhite::get_instance().blinking = false;

        TempCtl::get_instance().push(
            TempCtl::Controller::Events::TurnOn()
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

        TempCtl::get_instance().push(
            TempCtl::Controller::Events::TurnOff()
        );

        TempCtl::get_instance().push(
            self.menu_properties.running.value
            ? TempCtl::Controller::Events::Variant(TempCtl::Controller::Events::TurnOn())
            : TempCtl::Controller::Events::Variant(TempCtl::Controller::Events::TurnOff())
        );
    }

    void Panel::Menu::Actions::update_display_state(const DisplaySevmaps& display_sevmaps, panel::sevseg::green_yellow::MAX6549& max6549) {
        max6549.yellow_show(display_sevmaps.yellow);
        osDelay(1);
        max6549.green_show(display_sevmaps.green);
        osDelay(1);
        SevsegWhite::get_instance().push(display_sevmaps.white);
    }

    void Panel::Menu::Actions::turn_off(bool& on_off, panel::sevseg::green_yellow::MAX6549& max6549) {
        max6549.clear_all();
        osDelay(1);
        SevsegWhite::get_instance().push(panel::sevseg::common::sevmap());
        on_off = false;
    }

    void Panel::Menu::Actions::start_blinking() {
        SevsegWhite::get_instance().blinking = true;
    }

    void Panel::Menu::Actions::first_property(Properties::Pack::Variant& property, const Properties::Pack::Array& properties, panel::sevseg::green_yellow::MAX6549& max6549) {
        property = properties.front();
        std::visit([&](auto&& e) {
            update_display_state(e->to_display_state(), max6549);
        }, property);
    }

    void Panel::Menu::Actions::next_property(Properties::Pack::Variant& property, const Properties::Pack::Array& properties, panel::sevseg::green_yellow::MAX6549& max6549) {
        if(property.index() == (properties.size() - 1)) {
            property = properties.front();
        } else {
            property = properties[property.index() + 1];
        }
        std::visit([&](auto&& e) {
            update_display_state(e->to_display_state(), max6549);
        }, property);
    }

    void Panel::Menu::Actions::increment(Properties::Pack::Variant& property, panel::sevseg::green_yellow::MAX6549& max6549) {
        std::visit([&](auto&& e) {
            (*e)++;
            update_display_state(e->to_display_state(), max6549);
        }, property);
    }

    void Panel::Menu::Actions::decrement(Properties::Pack::Variant& property, panel::sevseg::green_yellow::MAX6549& max6549) {
        std::visit([&](auto&& e) {
            (*e)--;
            update_display_state(e->to_display_state(), max6549);
        }, property);
    }

    void Panel::Menu::Actions::next_step(Properties::Pack::Variant& property, panel::sevseg::green_yellow::MAX6549& max6549) {
        std::visit([&](auto&& e) {
            using Decay = std::remove_pointer_t<std::remove_reference_t<decltype(e)>>;
            if constexpr (
                std::is_same_v<Decay, Properties::Usings::DacFront>
                || std::is_same_v<Decay, Properties::Usings::DacRear>
            ) {
                e->next_step();
                update_display_state(e->to_display_state(), max6549);
            } else if constexpr (
                std::is_same_v<Decay, Properties::Usings::PeltierFront>
                || std::is_same_v<Decay, Properties::Usings::PeltierRear>
            ) {
                e->dac.next_step();
                update_display_state(e->to_display_state(), max6549);
            }
        }, property);
    }
}

namespace tasks {
    auto Panel::Menu::operator()() const {
		using namespace boost::sml;
        using namespace std;
        return make_transition_table(
            *state<States::OrthogonalSample> + event<Events::MAX31865_Sample> / function(Actions::save_sample<States::MAX31865, Events::MAX31865_Sample>) = state<States::OrthogonalSample>,
            state<States::OrthogonalSample> + event<Events::SHT31_InsideSample> / function(Actions::save_sample<States::SHT31_Inside, Events::SHT31_InsideSample>) = state<States::OrthogonalSample>,
            state<States::OrthogonalSample> + event<Events::SHT31_OutsideSample> / function(Actions::save_sample<States::SHT31_Outside, Events::SHT31_OutsideSample>) = state<States::OrthogonalSample>,

            *state<States::Off> + event<Events::TurnOn> / function(Actions::show_previous_sevmaps<States::MAX31865>) = state<States::MAX31865>,

            state<States::MAX31865> + event<Events::TurnOff> / function(Actions::turn_off) = state<States::Off>,
            state<States::MAX31865> + event<Events::MAX31865_Sample> / function(Actions::show_state<States::MAX31865>) = state<States::MAX31865>,
            state<States::MAX31865> + event<Events::NextItem> / function(Actions::show_previous_sevmaps<States::SHT31_Inside>) = state<States::SHT31_Inside>,

            state<States::SHT31_Inside> + event<Events::TurnOff> / function(Actions::turn_off) = state<States::Off>,
            state<States::SHT31_Inside> + event<Events::SHT31_InsideSample> / function(Actions::show_state<States::SHT31_Inside>) = state<States::SHT31_Inside>,
            state<States::SHT31_Inside> + event<Events::NextItem> / function(Actions::show_previous_sevmaps<States::SHT31_Outside>) = state<States::SHT31_Outside>,

            state<States::SHT31_Outside> + event<Events::TurnOff> / function(Actions::turn_off) = state<States::Off>,
            state<States::SHT31_Outside> + event<Events::SHT31_OutsideSample> / function(Actions::show_state<States::SHT31_Outside>) = state<States::SHT31_Outside>,
            state<States::SHT31_Outside> + event<Events::NextItem> / function(Actions::first_property) = state<States::Property>,

            state<States::Property> + event<Events::TurnOff> [Guards::is_not_blinking] / function(Actions::turn_off) = state<States::Off>,
            state<States::Property> + event<Events::NextItem> [Guards::is_last_property_and_is_not_blinking] / function(Actions::show_previous_sevmaps<States::MAX31865>) = state<States::MAX31865>,
            state<States::Property> + event<Events::NextItem> [Guards::is_not_blinking] / function(Actions::next_property) = state<States::Property>,

            state<States::Property> + event<Events::BlinkToggle> [Guards::is_blinking] / function(Actions::stop_blinking) = state<States::Property>,
            state<States::Property> + event<Events::BlinkToggle> / function(Actions::start_blinking) = state<States::Property>,
            state<States::Property> + event<Events::Increment> [Guards::is_blinking] / function(Actions::increment) = state<States::Property>,
            state<States::Property> + event<Events::Decrement> [Guards::is_blinking] / function(Actions::decrement) = state<States::Property>,
            state<States::Property> + event<Events::NextStep> / function(Actions::next_step) = state<States::Property>
        );
    }

    Panel::Panel() {
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