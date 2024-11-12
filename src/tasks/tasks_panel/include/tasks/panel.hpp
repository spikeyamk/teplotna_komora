#pragma once

#include <boost/sml.hpp>
#include <bitint.hpp>

#include "spi.h"
#include "actu/fan/ctl/ctl.hpp"
#include "tasks/temp_ctl.hpp"
#include "tasks/sevseg_white.hpp"
#include "panel/sevseg/common/common.hpp"
#include "panel/sevseg/green_yellow/green_yellow.hpp"
#include "tasks/sevseg_white.hpp"

namespace tasks {
    class Panel : public Task<Panel, 2 * 1024, "panel"> {
        friend CRTP;
    private:
        panel::sevseg::green_yellow::MAX6549 max6549 { &hspi2, SPI2_SEVYG_NSS_GPIO_Port, SPI2_SEVYG_NSS_Pin };

        struct DisplayState {
            panel::sevseg::common::sevmap white;
            panel::sevseg::common::sevmap yellow;
            panel::sevseg::common::sevmap green;
        };
    public:
        struct Menu {
            struct Properties {
                enum class Location {
                    DesiredRTD,
                    Running,
                    Algorithm,
                    Broiler,
                    Pump,
                    FanMaxRPM,
                    DacFront,
                    DacRear,
                    H_BridgeFront,
                    H_BridgeRear,
                };

                struct Detail {
                    template<typename T, Location location>
                    requires std::is_enum_v<Location>
                    class Property {
                    public:
                        using Type = T;
                        T& value;
                    public:
                        Property() = delete;
                        constexpr Property(T& value) :
                            value { value }
                        {}
                    };

                    template<sens::max31865::RTD min_clamp, sens::max31865::RTD max_clamp, sens::max31865::RTD step>
                    requires (
                        (step.adc_code.value > sens::max31865::ADC_Code::uint15_t(0))
                        && (max_clamp.adc_code.value > min_clamp.adc_code.value)
                    )
                    struct DesiredRTD : public Property<
                        sens::max31865::RTD,
                        Location::DesiredRTD
                    > {
                        DesiredRTD operator++(int) {
                            const DesiredRTD ret { *this };
                            if((this->value.adc_code.value + step.adc_code.value) <= max_clamp.adc_code.value) {
                                this->value.adc_code.value += step.adc_code.value;
                            }
                            return ret;
                        }

                        DesiredRTD operator--(int) {
                            const DesiredRTD ret { *this };
                            if((this->value.adc_code.value - step.adc_code.value) >= min_clamp.adc_code.value) {
                                this->value.adc_code.value -= step.adc_code.value;
                            }
                            return ret;
                        }

                        DisplayState to_display_state() {
                            return {
                                .white = panel::sevseg::common::to_sevmap(this->value.calculate_approx_temp().value()),
                                .yellow = panel::sevseg::common::to_sevmap<"temp">(),
                                .green = panel::sevseg::common::sevmap {},
                            };
                        }
                    };

                    template<util::TemplateStringNonNullTerminated name, Location location>
                    class Bool : public Property<bool, location> {
                    public:
                        constexpr Bool operator++(int) {
                            const Bool ret { *this };
                            (this->value) = !(this->value);
                            return ret;
                        }

                        constexpr Bool operator--(int) {
                            const Bool ret { *this };
                            (this->value) = !(this->value);
                            return ret;
                        }

                        DisplayState to_display_state() {
                            return {
                                .white = this->value ? panel::sevseg::common::to_sevmap<"true">() : panel::sevseg::common::to_sevmap<"false">(),
                                .yellow = panel::sevseg::common::to_sevmap<name>(),
                                .green = panel::sevseg::common::sevmap {},
                            };
                        }
                    };

                    template<util::TemplateStringNonNullTerminated name, Location location, auto first_state, decltype(first_state) second_state, decltype(first_state) ... rest_states>
                    requires (
                        std::is_enum_v<decltype(first_state)>
                        && util::is_unique<decltype(first_state), first_state, second_state, rest_states...>::value
                    )
                    class Enum : public Property<decltype(first_state), location> {
                    private:
                        static constexpr std::array<decltype(first_state), (sizeof...(rest_states)) + 2> array { first_state, second_state, rest_states... };
                    public:
                        constexpr Enum operator++(int) {
                            const Enum ret { *this };
                            const auto it { std::find(array.begin(), array.end(), this->value) };
                            if(it == (array.end() - 1)) {
                                this->value = array.front();
                            } else {
                                this->value = *(it + 1);
                            }
                            return ret;
                        }

                        constexpr Enum operator--(int) {
                            const Enum ret { *this };
                            const auto it { std::find(array.begin(), array.end(), this->value) };
                            if(it == array.begin()) {
                                this->value = array.back();
                            } else {
                                this->value = *(it - 1);
                            }
                            return ret;
                        }

                        DisplayState to_display_state() {
                            return {
                                .white = panel::sevseg::common::to_sevmap(panel::sevseg::common::uint20_t(static_cast<uint32_t>(this->value))),
                                .yellow = panel::sevseg::common::to_sevmap<name>(),
                                .green = panel::sevseg::common::sevmap {},
                            };
                        }
                    };

                    template<Location location>
                    class FanSpeedPercentage : public Property<actu::fan::ctl::SpeedPercentage, location> {
                    public:
                        constexpr FanSpeedPercentage operator++(int) {
                            const FanSpeedPercentage ret { *this };
                            return ret;
                        }

                        constexpr FanSpeedPercentage operator--(int) {
                            const FanSpeedPercentage ret { *this };
                            return ret;
                        }

                        DisplayState to_display_state() {
                            return {
                                .white = panel::sevseg::common::to_sevmap(panel::sevseg::common::uint20_t(static_cast<uint32_t>(this->value.unwrap()))),
                                .yellow = panel::sevseg::common::to_sevmap<"fan">(),
                                .green = panel::sevseg::common::sevmap {},
                            };
                        }
                    };

                    template<Location location>
                    class Dac : public Property<bitint::ubitint<12>, location> {
                    public:
                        constexpr Dac operator++(int) {
                            const Dac ret { *this };
                            return ret;
                        }

                        constexpr Dac operator--(int) {
                            const Dac ret { *this };
                            return ret;
                        }

                        DisplayState to_display_state() {
                            return {
                                .white = panel::sevseg::common::to_sevmap(panel::sevseg::common::uint20_t(static_cast<uint32_t>(this->value.unwrap()))),
                                .yellow = location == Location::DacFront ? panel::sevseg::common::to_sevmap<"dacf">() : panel::sevseg::common::to_sevmap<"dacr">(),
                                .green = panel::sevseg::common::sevmap {},
                            };
                        }
                    };
                };

                struct States {
                    enum class Algorithm {
                        Daniel,
                        SAR,
                        P,
                        PI,
                        PD,
                        PID,
                        FPCH,
                        Manual,
                    };

                    enum class H_Bridge {
                        Off,
                        Heat,
                        Cool,
                    };
                };

                struct Usings {
                    using Running = Detail::Bool<"run", Location::Running>;
                    using DesiredRTD = Detail::DesiredRTD<
                        sens::max31865::RTD(-20.0f),
                        sens::max31865::RTD(85.0f),
                        sens::max31865::RTD(
                            []() {
                                const sens::max31865::RTD base(0.0f);
                                sens::max31865::RTD ret(1.0f);
                                ret.adc_code.value -= base.adc_code.value;
                                return ret;
                            }()
                        )
                    >;
                    using Algorithm = Detail::Enum<
                        "algo",
                        Location::Algorithm,
                        TempCtl::Algorithm::Daniel,
                        TempCtl::Algorithm::SAR,
                        TempCtl::Algorithm::P,
                        TempCtl::Algorithm::PI,
                        TempCtl::Algorithm::PD,
                        TempCtl::Algorithm::PID,
                        TempCtl::Algorithm::Manual
                    >;
                    using Broiler = Detail::Bool<"broil", Location::Broiler>;
                    using Pump = Detail::Bool<"pump", Location::Pump>;
                    using FanMaxRPM = Detail::FanSpeedPercentage<Location::FanMaxRPM>;
                    using DacFront = Detail::Dac<Location::DacFront>;
                    using DacRear = Detail::Dac<Location::DacRear>;
                    template<util::TemplateStringNonNullTerminated name, Location location>
                    using H_Bridge = Detail::Enum<
                        name,
                        location,
                        TempCtl::H_Bridge::Off,
                        TempCtl::H_Bridge::Heat,
                        TempCtl::H_Bridge::Cool
                    >;
                    using H_BridgeFront = H_Bridge<"hbrdf", Location::H_BridgeFront>;
                    using H_BridgeRear = H_Bridge<"hbrdr", Location::H_BridgeRear>;
                    using Variant = std::variant<
                        Running,
                        DesiredRTD,
                        Algorithm,
                        Broiler,
                        Pump,
                        FanMaxRPM,
                        DacFront,
                        DacRear,
                        H_BridgeFront,
                        H_BridgeRear
                    >;
                };

                Usings::Running running { TempCtl::get_instance().running };
                Usings::DesiredRTD desired_rtd { TempCtl::get_instance().desired_rtd };
                Usings::Algorithm algorithm { TempCtl::get_instance().algorithm };
                Usings::Broiler broiler { TempCtl::get_instance().broiler };
                Usings::Pump pump { TempCtl::get_instance().pump };
                Usings::FanMaxRPM fan_max_rpm { TempCtl::get_instance().fan_max_rpm };
                Usings::DacFront dac_front { TempCtl::get_instance().dac_front };
                Usings::DacRear dac_rear { TempCtl::get_instance().dac_rear };
                Usings::H_BridgeFront hbridge_front { TempCtl::get_instance().hbridge_front };
                Usings::H_BridgeRear hbridge_rear { TempCtl::get_instance().hbridge_rear };

                using Variant = std::variant<
                    Usings::Running*,
                    Usings::DesiredRTD*,
                    Usings::Algorithm*,
                    Usings::Broiler*,
                    Usings::Pump*,
                    Usings::FanMaxRPM*,
                    Usings::DacFront*,
                    Usings::DacRear*,
                    Usings::H_BridgeFront*,
                    Usings::H_BridgeRear*
                >;

                std::array<Variant, 10> properties {
                    &running,
                    &desired_rtd,
                    &algorithm,
                    &broiler,
                    &pump,
                    &fan_max_rpm,
                    &dac_front,
                    &dac_rear,
                    &hbridge_front,
                    &hbridge_rear
                };
            };
                
            struct States {
                struct Off {};
                struct MeasuredTemp {};
                struct Property {};
            };

            struct Actions {
                static constexpr auto turn_on = [](Properties::Variant property, panel::sevseg::green_yellow::MAX6549& max6549) {
                    DisplayState display_state;
                    std::visit([&](auto&& e) {
                        (*e)++;
                        display_state = e->to_display_state();
                    }, property);
                    osDelay(1);
                    max6549.yellow_show(panel::sevseg::common::to_sevmap<"hello">());
                    osDelay(1);
                    max6549.green_show(panel::sevseg::common::to_sevmap<"ciao">());
                    osDelay(1);
                    SevsegWhite::get_instance().push(panel::sevseg::common::to_sevmap<"xoxo">());
                };

                static constexpr auto turn_off = [](panel::sevseg::green_yellow::MAX6549& max6549) {
                    max6549.clear_all();
                    osDelay(1);
                    SevsegWhite::get_instance().push(panel::sevseg::common::sevmap());
                };

                static constexpr auto start_blinking = []() {
                    SevsegWhite::get_instance().blinking = true;
                };

                static constexpr auto stop_blinking = []() {
                    SevsegWhite::get_instance().blinking = true;
                };

                static constexpr auto next = [](Properties::Variant property, const decltype(Properties::properties)& properties) {
                    if(property.index() == (properties.size() - 1)) {
                        property = properties.front();
                        return;
                    }
                    property = properties[property.index() + 1];
                };

                static constexpr auto increment = [](Properties::Variant property, panel::sevseg::green_yellow::MAX6549& max6549) {
                    DisplayState display_state;
                    std::visit([&](auto&& e) {
                        (*e)++;
                        display_state = e->to_display_state();
                    }, property);
                    osDelay(1);
                    max6549.yellow_show(display_state.yellow);
                    osDelay(1);
                    max6549.green_show(display_state.green);
                    osDelay(1);
                    SevsegWhite::get_instance().push(display_state.white);
                };

                static constexpr auto decrement = [](Properties::Variant property, panel::sevseg::green_yellow::MAX6549& max6549) {
                    DisplayState display_state;
                    std::visit([&](auto&& e) {
                        (*e)--;
                        display_state = e->to_display_state();
                    }, property);
                    osDelay(1);
                    max6549.yellow_show(display_state.yellow);
                    osDelay(1);
                    max6549.green_show(display_state.green);
                    osDelay(1);
                    SevsegWhite::get_instance().push(display_state.white);
                };
            };

            struct Events {
                struct TurnOn {};
                struct TurnOff {};

                struct Next {};

                struct StartBlink {};
                struct StopBlink {};
                struct Increment {};
                struct Decrement {};

                using Variant = std::variant<
                    TurnOn,
                    TurnOff,
                    Next,
                    StartBlink,
                    StopBlink,
                    Increment,
                    Decrement
                >;
            };

            struct Guards {
                static constexpr auto is_algorithm_manual = []() -> bool {
                    return TempCtl::get_instance().algorithm == TempCtl::Algorithm::Manual;
                };

                static constexpr auto is_blinking = []() {
                    return SevsegWhite::get_instance().blinking;
                };

                static constexpr auto is_last_property = []() {
                    return true;
                };
            };

            auto operator()() const;
        };
    private:
        std::array<Menu::Events::Variant, 8> events;
        StaticQueue_t queue_control_block {};
        osMessageQueueId_t queue { nullptr };
        Menu::Properties menu_properties {};
        Menu::Properties::Variant menu_property { menu_properties.properties.front() };
    private:
        Panel() = default;
    public:
        static Panel& get_instance();
        bool init();
    private:
        static void worker(void* arg);
    public:
        enum class Timeout {
            IRQ = 0,
            NotIRQ = 1'000,
        };
        osStatus_t push(const Menu::Events::Variant& event, const Timeout timeout);
        Menu::Events::Variant pop();
    };
}