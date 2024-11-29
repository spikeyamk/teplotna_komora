#pragma once

#include <variant>
#include <cmath>
#include <utility>
#include <boost/sml.hpp>

#include "cmsis_os2.h"
#include "actu/fan/ctl/ctl.hpp"
#include "actu/peltier/peltier.hpp"
#include "sens/max31865/rtd.hpp"
#include "sens/sht31/temp_hum.hpp"
#include "util/tmp.hpp"
#include "tasks/task.hpp"

namespace tasks {
    class TempCtl : public TaskBase<TempCtl, 4 * 1024, "temp_ctl"> {
        friend CRTP;
    public:
        enum class Algorithm {
            P,
            PI,
            PD,
            PID,
            PBIN,
            ManOp,
        };

        struct Controller {
            struct States {
                struct Off {};
                struct On {};
            };

            struct Events {
                struct TurnOn {};
                struct TurnOff {};

                struct Configuration {
                    sens::max31865::RTD desired_rtd;
                    bool broiler;
                    bool pump;
                    actu::fan::ctl::SpeedPercentage fan_max_rpm;
                    Algorithm algorithm;
                    bitint::ubitint<12> dac_front;
                    bitint::ubitint<12> dac_rear;
                    actu::peltier::hbridge::State hbridge_front;
                    actu::peltier::hbridge::State hbridge_rear;
                };

                struct MAX31865_Sample {
                    sens::max31865::RTD front;
                    sens::max31865::RTD rear;
                };

                struct SHT31_Sample {
                    sens::sht31::TempHum inside;
                    sens::sht31::TempHum outside;
                };

                using Variant = std::variant<
                    TurnOn,
                    TurnOff,
                    Configuration,
                    MAX31865_Sample,
                    SHT31_Sample 
                >;
            };

            struct Actions {
                static void turn_off(TempCtl& self);
                static void save_configuration(TempCtl& self, const Events::Configuration& event);

                template<typename Event>
                static void save_sample(TempCtl& self, const Event& event);

                static void run_algorithm(TempCtl& self, const Events::MAX31865_Sample& event);
            };

            auto operator()() const; 
        };
    private:
        std::array<Controller::Events::Variant, 10> events {};
        StaticQueue_t queue_control_block {};
        osMessageQueueId_t queue { nullptr };
    private:
        TempCtl();
    public:
        Controller::Events::Configuration configuration {
            .desired_rtd = 50.0f,
            .broiler = false,
            .pump = false,
            .fan_max_rpm = actu::fan::ctl::SpeedPercentage(30),
            .algorithm = Algorithm::P,
            .dac_front = 0,
            .dac_rear = 0,
            .hbridge_front = actu::peltier::hbridge::State::Off,
            .hbridge_rear = actu::peltier::hbridge::State::Off,
        };
        Controller::Events::MAX31865_Sample max31865_sample { sens::max31865::RTD(0.0f), sens::max31865::RTD(0.0f) };
        Controller::Events::SHT31_Sample sht31_sample { sens::sht31::TempHum(0, 0), sens::sht31::TempHum(0, 0) };

        static constexpr sens::max31865::RTD MAX_ALLOWED_RTD { 100.0f };
        static constexpr sens::max31865::RTD MIN_ALLOWED_RTD { -20.0f };

        static constexpr sens::max31865::RTD HEAT_MAX_DESIRED_RTD { 85.0f };
        static constexpr sens::max31865::RTD HEAT_MIN_DESIRED_RTD { 30.0f };
        static constexpr sens::max31865::RTD COOL_MAX_DESIRED_RTD { 10.0f };
        static constexpr sens::max31865::RTD COOL_MIN_DESIRED_RTD { -15.0f };

        static constexpr int32_t HEAT_MAX_ERR {
            static_cast<int32_t>(HEAT_MAX_DESIRED_RTD.adc_code.value.unwrap())
            - static_cast<int32_t>(MIN_ALLOWED_RTD.adc_code.value.unwrap())
        };

        static constexpr int32_t COOL_MIN_ERR {
            static_cast<int32_t>(COOL_MIN_DESIRED_RTD.adc_code.value.unwrap())
            - static_cast<int32_t>(MAX_ALLOWED_RTD.adc_code.value.unwrap())
        };

        struct PID {
            float p { static_cast<float>(0) };
            float i { static_cast<float>(0) };
            float d { static_cast<float>(0) };

            void reset() {
                p = static_cast<float>(0);
                i = static_cast<float>(0);
                d = static_cast<float>(0);
            }
        };
    public:
        struct Algorithms {
            struct Detail {
                template<auto current_source_set_func, auto hbridge_set_func>
                class ControlFunctor {
                public:
                    ControlFunctor() = delete;

                    static void run(
                        const actu::peltier::hbridge::State hbridge_state,
                        const actu::peltier::current_source::uint12_t dac_code
                    ) {
                        current_source_set_func(dac_code);
                        hbridge_set_func(hbridge_state);
                    }
                };

                using ControlFront = ControlFunctor<actu::peltier::current_source::front::set, actu::peltier::hbridge::front::set_state>;
                using ControlRear = ControlFunctor<actu::peltier::current_source::rear::set, actu::peltier::hbridge::rear::set_state>;

                class AlgorithmFunctor {
                public:
                    const sens::max31865::RTD& actual_rtd;
                    const Controller::Events::Configuration& configuration;
                    PID pid {};
                protected:
                    inline float err() const {
                        return static_cast<float>(
                            static_cast<int32_t>(configuration.desired_rtd.adc_code.value.unwrap())
                            - static_cast<int32_t>(actual_rtd.adc_code.value.unwrap())
                        );
                    }

                    inline float derr() const {
                        return err() - pid.p;
                    }

                    inline float ierr() const {
                        return pid.i + err();
                    }

                    enum class ActionType {
                        ShouldHeat,
                        ShouldCool,
                        Forbidden,
                    };

                    ActionType get_action() const {
                        if(
                            (configuration.desired_rtd.adc_code.value >= HEAT_MIN_DESIRED_RTD.adc_code.value)
                            && (configuration.desired_rtd.adc_code.value <= HEAT_MAX_DESIRED_RTD.adc_code.value)
                        ) {
                            return ActionType::ShouldHeat;
                        } else if(
                            (configuration.desired_rtd.adc_code.value >= COOL_MIN_DESIRED_RTD.adc_code.value)
                            && (configuration.desired_rtd.adc_code.value <= COOL_MAX_DESIRED_RTD.adc_code.value)
                        ) {
                            return ActionType::ShouldCool;
                        }

                        return ActionType::Forbidden;
                    }
                };

                template<typename Control, const uint32_t k_p>
                class P_Algorithm : public AlgorithmFunctor {
                public:
                    using Base = AlgorithmFunctor;
                    void run() {
                        this->pid.p = this->err();

                        const ActionType action_type { get_action() };
                        if(action_type == ActionType::ShouldHeat) {
                            const float k_p_heat { 20.0f };
                            Control::run(
                                actu::peltier::hbridge::State::Heat,
                                static_cast<uint16_t>(std::min(std::max(k_p_heat * this->pid.p, 0.0f), 4095.0f))
                            );
                        } else {
                            Control::run(
                                actu::peltier::hbridge::State::Off,
                                0
                            );
                        }

                        /*
                        if(rtd_diff >= 0.0f) {
                            const float k_p_heat { 20.0f };
                            Control::run(
                                actu::peltier::hbridge::State::Heat,
                                static_cast<uint16_t>(std::min(std::max(k_p_heat * rtd_diff, 0.0f), 4095.0f))
                            );
                        } else {
                            constexpr float k_p_cool { -1.0f / 4.0f };
                            Control::run(
                                actu::peltier::hbridge::State::Cool,
                                static_cast<uint16_t>(std::min(std::exp(k_p_cool * rtd_diff), 4095.0f))
                            );

                            constexpr float k_p_cool { -500.0f };
                            Control::run(
                                actu::peltier::hbridge::State::Cool,

                                static_cast<uint16_t>(std::min(std::max(k_p_heat * rtd_diff, 0.0f), 4095.0f))

                                static_cast<uint16_t>(std::min(k_p_cool*std::log(-rtd_diff), 4095.0f))
                            );
                        }
                        */
                    }
                private:
                    std::pair<actu::peltier::hbridge::State, actu::peltier::current_source::uint12_t> transfer_function() const {
                        const float tmp_result { this->err() };
                        return {
                            tmp_result > 0.0f ? actu::peltier::hbridge::State::Heat : actu::peltier::hbridge::State::Cool,
                            std::min(tmp_result, 4095.0f),
                        };
                    }
                };

                template<typename Control, const uint32_t k_p, const uint32_t k_d>
                class PD_Algorithm : public AlgorithmFunctor {
                public:
                    void run() {
                        this->pid.d = this->derr();
                        this->pid.p = this->err();

                        Control::run(
                            actu::peltier::hbridge::State::Off,
                            0
                        );
                    }
                };

                template<typename Control, const uint32_t k_p, const uint32_t k_i>
                class PI_Algorithm : public AlgorithmFunctor {
                public:
                    using Base = AlgorithmFunctor;
                    void run() {
                        this->pid.i += this->err();
                        this->pid.p = this->err();

                        const ActionType action_type { get_action() };
                        if(action_type == ActionType::ShouldHeat) {
                            const float k_p_heat { 20.0f };
                            const float k_i_heat { 0.000'5f };
                            Control::run(
                                actu::peltier::hbridge::State::Heat,
                                static_cast<uint16_t>(std::min(std::max(
                                    (k_p_heat * this->pid.p) + (k_i_heat * this->pid.i),
                                    0.0f
                                ), 4095.0f))
                            );
                        } else {
                            Control::run(
                                actu::peltier::hbridge::State::Off,
                                0
                            );
                        }
                    }

                    actu::peltier::current_source::uint12_t heat_integral_transfer_function() {
                        static constexpr int32_t FACTOR { (10 * 10'000) / 10 };

                        if(this->pid.i <= 0) {
                            return 0;
                        }

                        const int32_t tmp_result {
                            FACTOR * this->pid.i
                        };

                        if(tmp_result > 4095) {
                            return static_cast<uint16_t>(4095);
                        }

                        return tmp_result;
                    }

                    actu::peltier::current_source::uint12_t cool_integral_transfer_function() {
                        return 0;
                    }

                    actu::peltier::current_source::uint12_t heat_proportional_transfer_function() {
                        static constexpr int32_t FACTOR { 10 * 10'000 };

                        if(this->err() <= 0) {
                            return 0;
                        } else {
                            const int32_t tmp_result {
                                std::abs(
                                    (
                                        this->err()
                                        * FACTOR
                                    ) / 10'000
                                )
                            };

                            if(tmp_result > 4095) {
                                return static_cast<uint16_t>(4095);
                            }
                            
                            return tmp_result;
                        }
                    }

                    actu::peltier::current_source::uint12_t cool_proportional_transfer_function() {
                        if(this->err() >= 0) {
                            return 0;
                        } else {
                            return static_cast<uint16_t>(
                                std::abs(
                                    (
                                        this->err()
                                        * (
                                            (static_cast<int32_t>(bitint::ubitint<12>::max) * 10'000)
                                            / COOL_MIN_ERR
                                        )
                                    ) / 10'000
                                )
                            );
                        }
                    }
                };

                template<typename Control, const uint32_t k_p, const uint32_t k_i, const uint32_t k_d>
                class PID_Algorithm : public AlgorithmFunctor {
                public:
                    std::array<float, 1'000> err_samples { 0.0f };
                    decltype(err_samples.begin()) filler_it { err_samples.begin() };
                    decltype(err_samples.begin()) old_it { err_samples.begin() };
                public:
                    void reset() {
                        filler_it = err_samples.begin();
                    }

                    void run() {
                        this->pid.i += this->err();
                        this->pid.p = this->err();

                        if(filler_it != err_samples.end()) {
                            *filler_it = this->pid.p;
                            filler_it++;

                            this->pid.d = this->pid.p - *old_it;
                        } else {
                            this->pid.d = this->pid.p - *old_it;
                            *old_it = this->pid.p;
                            old_it++;
                            if(old_it == err_samples.end()) {
                                old_it = err_samples.begin();
                            }
                        }

                        const ActionType action_type { get_action() };
                        if(action_type == ActionType::ShouldHeat) {
                            const float k_p_heat { 15.0f };
                            const float k_i_heat { 0.000'35f };
                            const float k_d_heat { 12.0f }; // making this higher than this introduced SSE higher than desired RTD i.e. 0.9 °C with 20.0f

                            Control::run(
                                actu::peltier::hbridge::State::Heat,
                                static_cast<uint16_t>(std::min(std::max(
                                    (k_p_heat * this->pid.p)
                                    + (k_i_heat * this->pid.i)
                                    + (k_d_heat * this->pid.d)
                                    ,0.0f
                                ), 4095.0f))
                            );
                        } else {
                            Control::run(
                                actu::peltier::hbridge::State::Off,
                                0
                            );
                        }
                    }
                };

                template<typename Control>
                class PBIN_Algorithm : public AlgorithmFunctor {
                public:
                    void run() {
                        Control::run(
                            this->actual_rtd.adc_code.value > this->configuration.desired_rtd.adc_code.value
                            ? actu::peltier::hbridge::State::Cool
                            : actu::peltier::hbridge::State::Heat,
                            4095
                        );
                    }
                };
                
                template<typename Control>
                class ManOpAlgorithm : public AlgorithmFunctor {
                public:
                    void run() {
                        Control::run(
                            std::is_same_v<Control, ControlFront>
                            ? this->configuration.hbridge_front
                            : this->configuration.hbridge_rear
                            ,
                            std::is_same_v<Control, ControlRear>
                            ? this->configuration.dac_front
                            : this->configuration.dac_rear
                        );
                    }
                };

                template<typename AlgorithmFront, typename AlgorithmRear>
                class AlgorithmPair {
                public:
                    AlgorithmFront front;
                    AlgorithmRear rear;
                public:
                    AlgorithmPair() = delete;
                    AlgorithmPair(
                        const Controller::Events::MAX31865_Sample& max31865_sample,
                        const Controller::Events::Configuration& configuration
                    ) :
                        front { max31865_sample.front, configuration },
                        rear { max31865_sample.rear, configuration }
                    {}
                public:
                    void run() {
                        front.run();
                        rear.run(); 
                    }
                };
            };

            struct Usings {
                using P_AlgorithmFront = Detail::P_Algorithm<Detail::ControlFront, 1>;
                using P_AlgorithmRear = Detail::P_Algorithm<Detail::ControlRear, 1>;

                using PI_AlgorithmFront = Detail::PI_Algorithm<Detail::ControlFront, 1, 1>;
                using PI_AlgorithmRear = Detail::PI_Algorithm<Detail::ControlRear, 1, 1>;

                using PD_AlgorithmFront = Detail::PD_Algorithm<Detail::ControlFront, 1, 1>;
                using PD_AlgorithmRear = Detail::PD_Algorithm<Detail::ControlRear, 1, 1>;

                using PID_AlgorithmFront = Detail::PID_Algorithm<Detail::ControlFront, 1, 1, 1>;
                using PID_AlgorithmRear = Detail::PID_Algorithm<Detail::ControlRear, 1, 1, 1>;

                using PBIN_AlgorithmFront = Detail::PBIN_Algorithm<Detail::ControlFront>;
                using PBIN_AlgorithmRear = Detail::PBIN_Algorithm<Detail::ControlRear>;

                using ManOpAlgorithmFront = Detail::ManOpAlgorithm<Detail::ControlFront>;
                using ManOpAlgorithmRear = Detail::ManOpAlgorithm<Detail::ControlRear>;
            };

            class Pairs {
            public:
                using P_Pair = ::tasks::TempCtl::Algorithms::Detail::AlgorithmPair<Usings::P_AlgorithmFront, Usings::P_AlgorithmRear>;
                using PI_Pair = ::tasks::TempCtl::Algorithms::Detail::AlgorithmPair<Usings::PI_AlgorithmFront, Usings::PI_AlgorithmRear>;
                using PD_Pair = ::tasks::TempCtl::Algorithms::Detail::AlgorithmPair<Usings::PD_AlgorithmFront, Usings::PD_AlgorithmRear>;
                using PID_Pair = ::tasks::TempCtl::Algorithms::Detail::AlgorithmPair<Usings::PID_AlgorithmFront, Usings::PID_AlgorithmRear>;
                using FullPowerPair = ::tasks::TempCtl::Algorithms::Detail::AlgorithmPair<Usings::PBIN_AlgorithmFront, Usings::PBIN_AlgorithmRear>;
                using ManOpPair = ::tasks::TempCtl::Algorithms::Detail::AlgorithmPair<Usings::ManOpAlgorithmFront, Usings::ManOpAlgorithmRear>;

                using Variant = std::variant<
                    P_Pair*,
                    PI_Pair*,
                    PD_Pair*,
                    PID_Pair*,
                    FullPowerPair*,
                    ManOpPair*
                >;

                P_Pair p_pair;
                PI_Pair pi_pair;
                PD_Pair pd_pair;
                PID_Pair pid_pair;
                FullPowerPair fullpower_pair;
                ManOpPair manop_pair;

                std::array<Algorithms::Pairs::Variant, 6> array;

                decltype(array.begin()) active_pair;
            public:
                Pairs() = delete;

                Pairs(
                    const Controller::Events::MAX31865_Sample& max31865_sample,
                    const Controller::Events::Configuration& configuration
                ) :
                    p_pair { max31865_sample, configuration },
                    pi_pair { max31865_sample, configuration },
                    pd_pair { max31865_sample, configuration },
                    pid_pair { max31865_sample, configuration },
                    fullpower_pair { max31865_sample, configuration },
                    manop_pair { max31865_sample, configuration },
                    array {
                        &p_pair,
                        &pi_pair,
                        &pd_pair,
                        &pid_pair,
                        &fullpower_pair,
                        &manop_pair,
                    },
                    active_pair { array.begin() }
                {}
            };
        };

        Algorithms::Pairs algorithm_pairs { max31865_sample, configuration };
    public:
        static TempCtl& get_instance();
        osStatus_t push(const Controller::Events::Variant& event);
        Controller::Events::Variant pop();
    private:
        static void worker(void* arg);
        void apply_configuration();
        int32_t err(const sens::max31865::RTD& rtd) const;
    };
}