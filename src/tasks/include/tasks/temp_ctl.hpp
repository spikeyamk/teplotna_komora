#pragma once

#include <variant>
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
            Arbet,
            SAR,
            P,
            PD,
            PI,
            PID,
            FullPower,
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
        TempCtl() = default;
    public:
        Controller::Events::Configuration configuration {
            .desired_rtd = 40.0f,
            .broiler = false,
            .pump = false,
            .fan_max_rpm = actu::fan::ctl::SpeedPercentage(0),
            .algorithm = Algorithm::Arbet,
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
            int32_t p { 0 };
            int32_t i { 0 };
            int32_t d { 0 };

            void reset() {
                p = 0;
                i = 0;
                d = 0;
            }
        };
    public:
        struct Algorithms {
            struct Detail {
                template<auto current_source_set_func, auto hbridge_set_func>
                class ControlFunctor {
                    const actu::peltier::hbridge::State hbridge_state;
                    const actu::peltier::current_source::uint12_t dac_code;
                public:
                    ControlFunctor() = delete;
                    ControlFunctor(
                        const actu::peltier::hbridge::State hbridge_state,
                        const actu::peltier::current_source::uint12_t dac_code
                    ) :
                        hbridge_state { hbridge_state },
                        dac_code { dac_code }
                    {}

                    void run() {
                        current_source_set_func(dac_code);
                        hbridge_set_func(hbridge_state);
                    }
                };

                using ControlFront = ControlFunctor<actu::peltier::current_source::front::set, actu::peltier::hbridge::front::set_state>;
                using ControlRear = ControlFunctor<actu::peltier::current_source::rear::set, actu::peltier::hbridge::rear::set_state>;

                template<typename ControlFunctorSpecialized>
                class AlgorithmFunctor {
                public:
                    const sens::max31865::RTD& actual_rtd;
                    const Controller::Events::Configuration& configuration;
                    PID& pid;
                protected:
                    inline int32_t err() const {
                        return
                            static_cast<int32_t>(configuration.desired_rtd.adc_code.value.unwrap())
                            - static_cast<int32_t>(actual_rtd.adc_code.value.unwrap());
                    }

                    inline int32_t derr() const {
                        return err() - pid.p;
                    }

                    inline int32_t ierr() const {
                        return pid.i + err();
                    }

                    enum class ActionType {
                        ShouldHeat,
                        ShouldCool,
                        Forbidden,
                    };

                    ActionType get_action() {
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

                template<typename ControlFunctorSpecialized>
                class ArbetAlgorithm : public AlgorithmFunctor<ControlFunctorSpecialized> {
                public:
                    void run() {}
                };

                template<typename ControlFunctorSpecialized>
                class SAR_Algorithm : public AlgorithmFunctor<ControlFunctorSpecialized> {
                public:
                    void run() {}
                };

                template<typename ControlFunctorSpecialized, const uint32_t k_p>
                class P_Algorithm : public AlgorithmFunctor<ControlFunctorSpecialized> {
                public:
                    using Base = AlgorithmFunctor<ControlFunctorSpecialized>;
                    void run() {
                        this->pid.p = this->err();

                        switch(this->get_action()) {
                            default:
                                ControlFunctorSpecialized(
                                    actu::peltier::hbridge::State::Off,
                                    0
                                ).run();
                                return;
                            case Base::ActionType::ShouldHeat:
                                ControlFunctorSpecialized(
                                    actu::peltier::hbridge::State::Heat,
                                    heat_transfer_function()
                                ).run();
                                return;
                            case Base::ActionType::ShouldCool:
                                ControlFunctorSpecialized(
                                    actu::peltier::hbridge::State::Cool,
                                    cool_transfer_function()
                                ).run();
                                return;
                        }
                    }
                private:
                    actu::peltier::current_source::uint12_t heat_transfer_function() {
                        static constexpr int32_t FACTOR { 20 * 10'000 };

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

                    actu::peltier::current_source::uint12_t cool_transfer_function() {
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

                template<typename ControlFunctorSpecialized, const uint32_t k_p, const uint32_t k_d>
                class PD_Algorithm : public AlgorithmFunctor<ControlFunctorSpecialized> {
                public:
                    void run() {
                        this->pid.d = this->derr();
                        this->pid.p = this->err();

                        ControlFunctorSpecialized(
                            actu::peltier::hbridge::State::Off,
                            0
                        ).run();
                    }
                };

                template<typename ControlFunctorSpecialized, const uint32_t k_p, const uint32_t k_i>
                class PI_Algorithm : public AlgorithmFunctor<ControlFunctorSpecialized> {
                public:
                    using Base = AlgorithmFunctor<ControlFunctorSpecialized>;
                    void run() {
                        this->pid.i += this->err();
                        this->pid.p = this->err();

                        switch(this->get_action()) {
                            default:
                                ControlFunctorSpecialized(
                                    actu::peltier::hbridge::State::Off,
                                    0
                                ).run();
                                return;
                            case Base::ActionType::ShouldHeat:
                                ControlFunctorSpecialized(
                                    actu::peltier::hbridge::State::Heat,
                                    heat_proportional_transfer_function() + heat_integral_transfer_function()
                                ).run();
                                return;
                            case Base::ActionType::ShouldCool:
                                ControlFunctorSpecialized(
                                    actu::peltier::hbridge::State::Cool,
                                    cool_proportional_transfer_function()
                                ).run();
                                return;
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

                template<typename ControlFunctorSpecialized, const uint32_t k_p, const uint32_t k_i, const uint32_t k_d>
                class PID_Algorithm : public AlgorithmFunctor<ControlFunctorSpecialized> {
                public:
                    void run() {
                        this->pid.d = this->derr();
                        this->pid.i = this->ierr();
                        this->pid.p = this->err();

                        ControlFunctorSpecialized(
                            actu::peltier::hbridge::State::Off,
                            0
                        ).run();
                    }
                };

                template<typename ControlFunctorSpecialized>
                class FullPower_Algorithm : public AlgorithmFunctor<ControlFunctorSpecialized> {
                public:
                    void run() {
                        static size_t i = 0;
                        if(i % 100 == 0) {
                            std::cout
                                << "this->actual_rtd.adc_code.value: "
                                << this->actual_rtd.adc_code.value
                                << " this->configuration.desired_rtd.adc_code.value: "
                                << this->configuration.desired_rtd.adc_code.value
                                << std::endl;
                        }
                        i++;

                        ControlFunctorSpecialized(
                            this->actual_rtd.adc_code.value > this->configuration.desired_rtd.adc_code.value
                            ? actu::peltier::hbridge::State::Cool
                            : actu::peltier::hbridge::State::Heat,
                            4095
                        ).run();
                    }
                };
                
                template<typename ControlFunctorSpecialized>
                class ManOpAlgorithm : public AlgorithmFunctor<ControlFunctorSpecialized> {
                public:
                    void run() {
                        ControlFunctorSpecialized(
                            std::is_same_v<ControlFunctorSpecialized, ControlFront>
                            ? this->configuration.hbridge_front
                            : this->configuration.hbridge_rear
                            ,
                            std::is_same_v<ControlFunctorSpecialized, ControlFront>
                            ? this->configuration.dac_front
                            : this->configuration.dac_rear
                        ).run();
                    }
                };

                template<typename AlgorithmFront, typename AlgorithmRear>
                class AlgorithmPair {
                private:
                    const Controller::Events::MAX31865_Sample& max31865_sample;
                    const Controller::Events::Configuration& configuration;
                    PID& pid_front;
                    PID& pid_rear;
                public:
                    AlgorithmPair() = delete;
                    inline AlgorithmPair(
                        const Controller::Events::MAX31865_Sample& max31865_sample,
                        const Controller::Events::Configuration& configuration,
                        PID& pid_front,
                        PID& pid_rear
                    ) :
                        max31865_sample { max31865_sample },
                        configuration { configuration },
                        pid_front { pid_front },
                        pid_rear { pid_rear }
                    {}

                    void run() {
                        AlgorithmFront front  { max31865_sample.front, configuration, pid_front };
                        front.run();
                        AlgorithmRear rear { max31865_sample.rear, configuration, pid_rear };
                        rear.run(); 
                    }
                };
            };

            struct Usings {
                using ArbetAlgorithmFront = Detail::ArbetAlgorithm<Detail::ControlFront>;
                using ArbetAlgorithmRear = Detail::ArbetAlgorithm<Detail::ControlRear>;

                using SAR_AlgorithmFront = Detail::SAR_Algorithm<Detail::ControlFront>;
                using SAR_AlgorithmRear = Detail::SAR_Algorithm<Detail::ControlRear>;

                using P_AlgorithmFront = Detail::P_Algorithm<Detail::ControlFront, 1>;
                using P_AlgorithmRear = Detail::P_Algorithm<Detail::ControlRear, 1>;

                using PD_AlgorithmFront = Detail::PD_Algorithm<Detail::ControlFront, 1, 1>;
                using PD_AlgorithmRear = Detail::PD_Algorithm<Detail::ControlRear, 1, 1>;

                using PI_AlgorithmFront = Detail::PI_Algorithm<Detail::ControlFront, 1, 1>;
                using PI_AlgorithmRear = Detail::PI_Algorithm<Detail::ControlRear, 1, 1>;

                using PID_AlgorithmFront = Detail::PID_Algorithm<Detail::ControlFront, 1, 1, 1>;
                using PID_AlgorithmRear = Detail::PID_Algorithm<Detail::ControlRear, 1, 1, 1>;

                using FullPower_AlgorithmFront = Detail::FullPower_Algorithm<Detail::ControlFront>;
                using FullPower_AlgorithmRear = Detail::FullPower_Algorithm<Detail::ControlRear>;

                using ManOpAlgorithmFront = Detail::ManOpAlgorithm<Detail::ControlFront>;
                using ManOpAlgorithmRear = Detail::ManOpAlgorithm<Detail::ControlRear>;
            };

            class Pairs {
            public:
                using ArbetPair = Detail::AlgorithmPair<Usings::ArbetAlgorithmFront, Usings::ArbetAlgorithmRear>;
                using SAR_Pair = Detail::AlgorithmPair<Usings::SAR_AlgorithmFront, Usings::SAR_AlgorithmRear>;
                using P_Pair = Detail::AlgorithmPair<Usings::P_AlgorithmFront, Usings::P_AlgorithmRear>;
                using PD_Pair = Detail::AlgorithmPair<Usings::PD_AlgorithmFront, Usings::PD_AlgorithmRear>;
                using PI_Pair = Detail::AlgorithmPair<Usings::PI_AlgorithmFront, Usings::PI_AlgorithmRear>;
                using PID_Pair = Detail::AlgorithmPair<Usings::PID_AlgorithmFront, Usings::PID_AlgorithmRear>;
                using FullPowerPair = Detail::AlgorithmPair<Usings::FullPower_AlgorithmFront, Usings::FullPower_AlgorithmRear>;
                using ManOpPair = Detail::AlgorithmPair<Usings::ManOpAlgorithmFront, Usings::ManOpAlgorithmRear>;

                using Variant = std::variant<
                    ArbetPair*,
                    SAR_Pair*,
                    P_Pair*,
                    PD_Pair*,
                    PI_Pair*,
                    PID_Pair*,
                    FullPowerPair*,
                    ManOpPair*
                >;

                ArbetPair arbet_pair;
                SAR_Pair sar_pair;
                P_Pair p_pair;
                PD_Pair pd_pair;
                PI_Pair pi_pair;
                PID_Pair pid_pair;
                FullPowerPair fullpower_pair;
                ManOpPair manop_pair;
            public:
                Pairs() = delete;

                Pairs(
                    const Controller::Events::MAX31865_Sample& max31865_sample,
                    const Controller::Events::Configuration& configuration,
                    PID& pid_front,
                    PID& pid_rear
                ) :
                    arbet_pair { max31865_sample, configuration, pid_front, pid_rear },
                    sar_pair { max31865_sample, configuration, pid_front, pid_rear },
                    p_pair { max31865_sample, configuration, pid_front, pid_rear },
                    pd_pair { max31865_sample, configuration, pid_front, pid_rear },
                    pi_pair { max31865_sample, configuration, pid_front, pid_rear },
                    pid_pair { max31865_sample, configuration, pid_front, pid_rear },
                    fullpower_pair { max31865_sample, configuration, pid_front, pid_rear },
                    manop_pair { max31865_sample, configuration, pid_front, pid_rear }
                {}
            };
        };

        PID pid_front {};
        PID pid_rear {};

        Algorithms::Pairs algorithm_pairs { max31865_sample, configuration, pid_front, pid_rear };
        std::array<Algorithms::Pairs::Variant, 8> algorithm_pairs_array {
            &algorithm_pairs.arbet_pair,
            &algorithm_pairs.sar_pair,
            &algorithm_pairs.p_pair,
            &algorithm_pairs.pd_pair,
            &algorithm_pairs.pi_pair,
            &algorithm_pairs.pid_pair,
            &algorithm_pairs.fullpower_pair,
            &algorithm_pairs.manop_pair,
        };
        decltype(algorithm_pairs_array.begin()) algorithm_pairs_active_algorithm { algorithm_pairs_array.begin() };
    public:
        static TempCtl& get_instance();
        bool init();
        osStatus_t push(const Controller::Events::Variant& event);
        Controller::Events::Variant pop();
    private:
        static void worker(void* arg);
        void apply_configuration();
        int32_t err(const sens::max31865::RTD& rtd) const;
    };
}