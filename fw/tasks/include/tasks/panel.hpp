#pragma once

#include <boost/sml.hpp>
#include <bitint.hpp>

#include "spi.h"
#include "actu/fan/ctl/ctl.hpp"
#include "tasks/temp_ctl.hpp"
#include "tasks/sevseg_white.hpp"
#include "panel/sevseg/common/common.hpp"
#include "panel/sevseg/green_yellow/green_yellow.hpp"
#include "sens/max31865/rtd.hpp"
#include "sens/sht31/temp_hum.hpp"
#include "tasks/sevseg_white.hpp"

namespace tasks {
    class Panel : public TaskBase<Panel, 4 * 1024, "panel"> {
        friend CRTP;
    private:
        panel::sevseg::green_yellow::MAX6549 max6549 { &hspi2, SPI2_SEVYG_NSS_GPIO_Port, SPI2_SEVYG_NSS_Pin };

        struct DisplaySevmaps {
            panel::sevseg::common::sevmap white;
            panel::sevseg::common::sevmap yellow;
            panel::sevseg::common::sevmap green;
        };
    public:
        struct Menu {
            struct Properties {
                struct Detail {
                    template<typename T>
                    class Property {
                    public:
                        using Type = T;
                        T value;
                    public:
                        Property() = delete;
                        constexpr Property(T value) :
                            value { value }
                        {}
                    };

                    template<sens::max31865::RTD min_clamp, sens::max31865::RTD max_clamp, sens::max31865::RTD step>
                    requires (
                        (step.adc_code.value > sens::max31865::ADC_Code::uint15_t(0))
                        && (max_clamp.adc_code.value > min_clamp.adc_code.value)
                    )
                    struct RTD_Property : public Property<sens::max31865::RTD> {
                        RTD_Property operator++(int) {
                            const RTD_Property ret { *this };
                            if((this->value.adc_code.value + step.adc_code.value) <= max_clamp.adc_code.value) {
                                this->value.adc_code.value += step.adc_code.value;
                            }
                            return ret;
                        }

                        RTD_Property operator--(int) {
                            const RTD_Property ret { *this };
                            if((this->value.adc_code.value - step.adc_code.value) >= min_clamp.adc_code.value) {
                                this->value.adc_code.value -= step.adc_code.value;
                            }
                            return ret;
                        }

                        DisplaySevmaps to_display_state() {
                            return {
                                .white = panel::sevseg::common::to_sevmap(this->value.calculate_approx_temp().value()),
                                .yellow = panel::sevseg::common::to_sevmap<"temp">(),
                                .green = panel::sevseg::common::sevmap {},
                            };
                        }
                    };

                    template<util::TemplateStringNonNullTerminated name>
                    class BoolProperty : public Property<bool> {
                    public:
                        constexpr BoolProperty operator++(int) {
                            const BoolProperty ret { *this };
                            this->value = true;
                            return ret;
                        }

                        constexpr BoolProperty operator--(int) {
                            const BoolProperty ret { *this };
                            this->value = false;
                            return ret;
                        }

                        DisplaySevmaps to_display_state() {
                            return {
                                .white = this->value ? panel::sevseg::common::to_sevmap<"true">() : panel::sevseg::common::to_sevmap<"false">(),
                                .yellow = panel::sevseg::common::to_sevmap<name>(),
                                .green = panel::sevseg::common::sevmap {},
                            };
                        }
                    };

                    template<
                        util::TemplateStringNonNullTerminated name,
                        typename T,
                        util::EnumHolder first_state,
                        util::EnumHolder second_state,
                        util::EnumHolder ... rest_states
                    >
                    requires(util::is_unique_by_type<decltype(first_state), decltype(second_state), decltype(rest_states)...>::value)
                    class EnumProperty : public Property<T> {
                    public:
                        using Variant = std::variant<
                            decltype(first_state),
                            decltype(second_state),
                            decltype(rest_states)...
                        >;
                    private:
                        static constexpr std::array<Variant, sizeof...(rest_states) + 2> state_variants {
                            first_state,
                            second_state,
                            rest_states... 
                        };

                        constexpr auto find_current_it() {
                            return std::find_if(state_variants.begin(), state_variants.end(), [&](const auto& state_variant) {
                                bool is_equal { false };
                                std::visit([&](auto&& state) {
                                    is_equal = (state.value == this->value);
                                }, state_variant);
                                return is_equal;
                            });
                        }
                    public:
                        constexpr EnumProperty operator++(int) {
                            const EnumProperty ret { *this };
                            const auto it { find_current_it() };
                            if(it == (state_variants.end() - 1)) {
                                return ret;
                            }
                            std::visit([&](auto&& v) {
                                this->value = v.value;
                            }, *(it + 1));
                            return ret;
                        }

                        constexpr EnumProperty operator--(int) {
                            const EnumProperty ret { *this };
                            const auto it { find_current_it() };
                            if(it == state_variants.begin()) {
                                return ret;
                            }
                            std::visit([&](auto&& v) {
                                this->value = v.value;
                            }, *(it - 1));
                            return ret;
                        }

                        DisplaySevmaps to_display_state() {
                            DisplaySevmaps ret {
                                .white = panel::sevseg::common::sevmap {},
                                .yellow = panel::sevseg::common::to_sevmap<name>(),
                                .green = panel::sevseg::common::sevmap {},
                            };

                            std::visit([&](auto&& v) {
                                ret.white = panel::sevseg::common::to_sevmap<v.name>();
                            }, *find_current_it());

                            return ret;
                        }
                    };

                    template<util::TemplateStringNonNullTerminated name>
                    class FanSpeedPercentage : public Property<actu::fan::ctl::SpeedPercentage> {
                    public:
                        constexpr FanSpeedPercentage operator++(int) {
                            const FanSpeedPercentage ret { *this };
                            this->value++;
                            return ret;
                        }

                        constexpr FanSpeedPercentage operator--(int) {
                            const FanSpeedPercentage ret { *this };
                            this->value--;
                            return ret;
                        }

                        DisplaySevmaps to_display_state() {
                            return {
                                .white = panel::sevseg::common::to_sevmap(static_cast<float>(this->value.unwrap_less_is_less())),
                                .yellow = panel::sevseg::common::to_sevmap<name>(),
                                .green = panel::sevseg::common::sevmap {},
                            };
                        }
                    };

                    template<util::TemplateStringNonNullTerminated name>
                    class Dac : public Property<bitint::ubitint<12>> {
                    public:
                        static constexpr std::array<bitint::ubitint<12>, 4> steps {
                            1, 10, 100, 1'000
                        };
                        decltype(steps.begin()) step_it;
                        using Base = Property<bitint::ubitint<12>>;
                    public:
                        constexpr Dac(bitint::ubitint<12>& value) :
                            Base { value },
                            step_it { steps.begin() }
                        {}

                        constexpr Dac operator++(int) {
                            const Dac ret { *this };
                            if((this->value.unwrap() + this->step_it->unwrap()) > bitint::ubitint<12>::max) {
                                this->value = bitint::ubitint<12>::max;
                                return ret;
                            }

                            this->value += *(this->step_it);
                            return ret;
                        }

                        constexpr Dac operator--(int) {
                            const Dac ret { *this };

                            if(*this->step_it > this->value) {
                                this->value = bitint::ubitint<12>::min;
                                return ret;
                            }

                            this->value -= *(this->step_it);
                            return ret;
                        }

                        DisplaySevmaps to_display_state() {
                            return {
                                .white = panel::sevseg::common::to_sevmap(panel::sevseg::common::int14_t(static_cast<int16_t>(this->value.unwrap()))),
                                .yellow = panel::sevseg::common::to_sevmap<name>(),
                                .green = panel::sevseg::common::to_sevmap(panel::sevseg::common::int14_t(static_cast<int16_t>(step_it->unwrap()))),
                            };
                        }

                        constexpr void next_step() {
                            if((step_it + 1) == steps.end()) {
                                step_it = steps.begin();
                                return;
                            }

                            step_it++;
                        }
                    };

                    template<util::TemplateStringNonNullTerminated name>
                    using H_Bridge = Detail::EnumProperty<
                        name,
                        actu::peltier::hbridge::State,
                        util::EnumHolder<"off", actu::peltier::hbridge::State::Off> {},
                        util::EnumHolder<"heat", actu::peltier::hbridge::State::Heat> {},
                        util::EnumHolder<"cool", actu::peltier::hbridge::State::Cool> {}
                    >;

                    template<util::TemplateStringNonNullTerminated name, typename DacPropertySpecialized, typename H_BridgePropertySpecialized>
                    class Peltier {
                    public:
                        DacPropertySpecialized& dac;
                        H_BridgePropertySpecialized& hbridge;
                    public: 
                        Peltier() = delete;
                        constexpr Peltier(DacPropertySpecialized& dac, H_BridgePropertySpecialized& hbridge) :
                            dac { dac },
                            hbridge { hbridge }
                        {}
                    private:
                        void reset() {
                            hbridge.value = actu::peltier::hbridge::State::Off;
                            dac.value = bitint::ubitint<12>(static_cast<uint16_t>(0));
                        }

                        constexpr bool get_unavailable() {
                            return (
                                (
                                    (hbridge.value == actu::peltier::hbridge::State::Off)
                                    && (dac.value != bitint::ubitint<12>(static_cast<uint16_t>(0)))
                                )
                                || (
                                    (dac.value == bitint::ubitint<12>(static_cast<uint16_t>(0)))
                                    && (hbridge.value != actu::peltier::hbridge::State::Off)
                                )
                            );
                        }
                    public:
                        constexpr Peltier operator++(int) {
                            const Peltier ret { *this };
                            if(get_unavailable()) {
                                return ret;
                            }

                            if(hbridge.value == actu::peltier::hbridge::State::Off) {
                                hbridge.value = actu::peltier::hbridge::State::Heat;
                                dac.value = *dac.step_it;
                                return ret;
                            }

                            if(hbridge.value == actu::peltier::hbridge::State::Cool) {
                                if(dac.value <= *dac.step_it) {
                                    reset();
                                    return ret;
                                }
                                dac--;
                                return ret;
                            }

                            dac++;
                            return ret;
                        }

                        constexpr Peltier operator--(int) {
                            const Peltier ret { *this };
                            if(get_unavailable()) {
                                return ret;
                            }

                            if(hbridge.value == actu::peltier::hbridge::State::Off) {
                                hbridge.value = actu::peltier::hbridge::State::Cool;
                                dac.value = *dac.step_it;
                                return ret;
                            }

                            if(hbridge.value == actu::peltier::hbridge::State::Heat) {
                                if(dac.value <= *dac.step_it) {
                                    reset();
                                    return ret;
                                }
                                dac--;
                                return ret;
                            }

                            dac++;
                            return ret;
                        }

                        DisplaySevmaps to_display_state() {
                            return {
                                .white = (
                                    get_unavailable()
                                    ? panel::sevseg::common::to_sevmap<"unavl">()
                                    : (
                                        panel::sevseg::common::to_sevmap(
                                            hbridge.value == actu::peltier::hbridge::State::Off
                                            ? 0.0f
                                            : (
                                                hbridge.value == actu::peltier::hbridge::State::Heat
                                                ? panel::sevseg::common::int14_t(static_cast<int16_t>(dac.value.unwrap()))
                                                : panel::sevseg::common::int14_t(-static_cast<int16_t>(dac.value.unwrap()))
                                            )
                                        )
                                    )
                                ),
                                .yellow = panel::sevseg::common::to_sevmap<name>(),
                                .green = panel::sevseg::common::to_sevmap(panel::sevseg::common::int14_t(static_cast<int16_t>(dac.step_it->unwrap()))),
                            };
                        } 
                    };
                };

                struct Usings {
                    using Running = Detail::BoolProperty<"run">;
                    using DesiredRTD = Detail::RTD_Property<
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
                    using Algorithm = Detail::EnumProperty<
                        "algo",
                        TempCtl::Algorithm,
                        util::EnumHolder<"P", TempCtl::Algorithm::P> {},
                        util::EnumHolder<"PI", TempCtl::Algorithm::PI> {},
                        util::EnumHolder<"PD", TempCtl::Algorithm::PD> {},
                        util::EnumHolder<"PID", TempCtl::Algorithm::PID> {},
                        util::EnumHolder<"PBIN", TempCtl::Algorithm::PBIN> {},
                        util::EnumHolder<"ManOp", TempCtl::Algorithm::ManOp> {}
                    >;
                    using Broiler = Detail::BoolProperty<"broil">;
                    using Pump = Detail::BoolProperty<"pump">;
                    using FanMaxRPM = Detail::FanSpeedPercentage<"fan">;
                    using DacFront = Detail::Dac<"dacf">;
                    using DacRear = Detail::Dac<"dacr">;
                    using H_BridgeFront = Detail::H_Bridge<"hbrdf">;
                    using H_BridgeRear = Detail::H_Bridge<"hbrdr">;
                    using PeltierFront = Detail::Peltier<"peltf", DacFront, H_BridgeFront>;
                    using PeltierRear = Detail::Peltier<"peltr", DacRear, H_BridgeRear>;
                };

                Usings::Running running { false };
                Usings::DesiredRTD desired_rtd { TempCtl::get_instance().configuration.desired_rtd };
                Usings::Broiler broiler { TempCtl::get_instance().configuration.broiler };
                Usings::Pump pump { TempCtl::get_instance().configuration.pump };
                Usings::FanMaxRPM fan_max_rpm { TempCtl::get_instance().configuration.fan_max_rpm };
                Usings::Algorithm algorithm { TempCtl::get_instance().configuration.algorithm };
                Usings::DacFront dac_front { TempCtl::get_instance().configuration.dac_front };
                Usings::DacRear dac_rear { TempCtl::get_instance().configuration.dac_rear };
                Usings::H_BridgeFront hbridge_front { TempCtl::get_instance().configuration.hbridge_front };
                Usings::H_BridgeRear hbridge_rear { TempCtl::get_instance().configuration.hbridge_rear };
                Usings::PeltierFront peltier_front { dac_front, hbridge_front };
                Usings::PeltierRear peltier_rear { dac_rear, hbridge_rear };

                template<typename ... Args>
                requires util::is_unique_by_type<Args...>::value
                struct PackHolder {
                    using Variant = std::variant<Args...>;
                    using Array = std::array<Variant, 12>;
                    static constexpr size_t algorithm_index { Variant(static_cast<Usings::Algorithm*>(nullptr)).index()  };
                };

                using Pack = PackHolder<
                    Usings::Running*,
                    Usings::DesiredRTD*,
                    Usings::Broiler*,
                    Usings::Pump*,
                    Usings::FanMaxRPM*,
                    Usings::Algorithm*,
                    Usings::DacFront*,
                    Usings::DacRear*,
                    Usings::H_BridgeFront*,
                    Usings::H_BridgeRear*,
                    Usings::PeltierFront*,
                    Usings::PeltierRear*
                >;

                Pack::Array properties {
                    &running,
                    &desired_rtd,
                    &broiler,
                    &pump,
                    &fan_max_rpm,
                    &algorithm,
                    &dac_front,
                    &dac_rear,
                    &hbridge_front,
                    &hbridge_rear,
                    &peltier_front,
                    &peltier_rear,
                };
            };

            struct Events {
                struct TurnOn {};
                struct TurnOff {};

                class MAX31865_Sample {
                private:
                    sens::max31865::RTD rtd_front;
                    sens::max31865::RTD rtd_rear;
                public:
                    MAX31865_Sample(const sens::max31865::RTD& rtd_front, const sens::max31865::RTD& rtd_rear) :
                        rtd_front { rtd_front },
                        rtd_rear { rtd_rear }
                    {}

                    DisplaySevmaps to_display_state() const {
                        return {
                            .white = panel::sevseg::common::to_sevmap<"maxfr">(),
                            .yellow = panel::sevseg::common::to_sevmap(rtd_front.calculate_approx_temp().value()),
                            .green = panel::sevseg::common::to_sevmap(rtd_rear.calculate_approx_temp().value()),
                        };
                    }
                };

                template<util::TemplateStringNonNullTerminated name>
                class SHT31_Sample {
                private:
                    sens::sht31::TempHum temp_hum;
                public:
                    SHT31_Sample(const sens::sht31::TempHum& temp_hum) :
                        temp_hum { temp_hum }
                    {}

                    DisplaySevmaps to_display_state() const {
                        return {
                            .white = panel::sevseg::common::to_sevmap<name>(),
                            .yellow = panel::sevseg::common::to_sevmap(temp_hum.calculate_temp()),
                            .green = panel::sevseg::common::to_sevmap(temp_hum.calculate_hum()),
                        };
                    }
                };

                using SHT31_InsideSample = SHT31_Sample<"insth">;
                using SHT31_OutsideSample = SHT31_Sample<"outth">;

                struct NextItem {};

                struct BlinkToggle {};
                struct Increment {};
                struct Decrement {};
                struct NextStep {};

                using Variant = std::variant<
                    TurnOn,
                    TurnOff,
                    MAX31865_Sample,
                    SHT31_InsideSample,
                    SHT31_OutsideSample,
                    NextItem,
                    BlinkToggle,
                    Increment,
                    Decrement,
                    NextStep
                >;
            };
                
            struct States {
                struct Off {};

                struct MAX31865 {
                    std::optional<Events::MAX31865_Sample> event { std::nullopt };
                    std::optional<DisplaySevmaps> display_sevmaps { std::nullopt };
                    static constexpr size_t sample_threshold { 1 };
                    size_t sample_counter { sample_threshold - 1 };
                };

                struct Detail {
                    template<typename SampleType>
                    struct SHT31 {
                        std::optional<SampleType> event { std::nullopt };
                        std::optional<DisplaySevmaps> display_sevmaps { std::nullopt };
                        static constexpr size_t sample_threshold { 1 };
                        size_t sample_counter { sample_threshold - 1 };
                    };
                };

                using SHT31_Inside = Detail::SHT31<Events::SHT31_InsideSample>;
                using SHT31_Outside = Detail::SHT31<Events::SHT31_OutsideSample>;

                struct Property {};

                struct OrthogonalSample {};
            };

            struct Actions {
                static void update_display_state(const DisplaySevmaps& display_sevmaps, panel::sevseg::green_yellow::MAX6549& max6549);

                template<typename State>
                static void show_previous_sevmaps(State& state, bool& on_off, panel::sevseg::green_yellow::MAX6549& max6549) {
                    update_display_state(state.display_sevmaps.value_or(DisplaySevmaps{}), max6549);
                    on_off = true;
                }

                static void turn_off(bool& on_off, panel::sevseg::green_yellow::MAX6549& max6549);
                static void start_blinking();
                static void stop_blinking(Panel& self);
                static void first_property(Properties::Pack::Variant& property, const Properties::Pack::Array& properties, panel::sevseg::green_yellow::MAX6549& max6549);
                static void next_property(Properties::Pack::Variant& property, const Properties::Pack::Array& properties, panel::sevseg::green_yellow::MAX6549& max6549);

                template<typename State, typename Sample>
                static void save_sample(State& state, const Sample& event) {
                    state.event = event;
                    if(state.display_sevmaps.has_value() == false) {
                        state.display_sevmaps = event.to_display_state();
                    }
                }

                template<typename State>
                static void show_state(State& state, panel::sevseg::green_yellow::MAX6549& max6549) {
                    state.sample_counter++;
                    if(state.sample_counter == state.sample_threshold) {
                        if(state.event.has_value()) {
                            state.display_sevmaps = state.event.value().to_display_state();
                        }
                        update_display_state(state.display_sevmaps.value_or(DisplaySevmaps{}), max6549);
                        state.sample_counter = 0;
                    }
                }

                static void increment(Properties::Pack::Variant& property, panel::sevseg::green_yellow::MAX6549& max6549);
                static void decrement(Properties::Pack::Variant& property, panel::sevseg::green_yellow::MAX6549& max6549);
                static void next_step(Properties::Pack::Variant& property, panel::sevseg::green_yellow::MAX6549& max6549);
            };

            struct Guards {
                static constexpr auto is_algorithm_manop = []() {
                    return TempCtl::get_instance().configuration.algorithm == TempCtl::Algorithm::ManOp;
                };

                static constexpr auto is_blinking = []() {
                    return SevsegWhite::get_instance().blinking;
                };

                static constexpr auto is_not_blinking = []() {
                    return !SevsegWhite::get_instance().blinking;
                };

                static constexpr auto is_last_property_and_is_not_blinking = [](const Properties::Pack::Variant& property, const Properties::Pack::Array& properties) {
                    const size_t stopper_index {
                        [&properties]() -> size_t {
                            if(is_algorithm_manop()) {
                                return properties.size() - 1;
                            }
                            return Properties::Pack::algorithm_index;
                        }()
                    };

                    return (
                        (property.index() == stopper_index)
                        && is_not_blinking()
                    );
                };
            };

            auto operator()() const;
        };
    public: 
        bool on_off { false };
    private:
        std::array<Menu::Events::Variant, 8> events;
        StaticQueue_t queue_control_block {};
        osMessageQueueId_t queue { nullptr };
        Menu::Properties menu_properties {};
        Menu::Properties::Pack::Variant menu_property { menu_properties.properties.front() };
    public:
        Menu::States::MAX31865 max31865 {};
        Menu::States::SHT31_Inside sht31_inside {};
        Menu::States::SHT31_Outside sht31_outside {};
    private:
        Panel();
    public:
        static Panel& get_instance();
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