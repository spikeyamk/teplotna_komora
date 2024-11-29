#pragma once

#include <iostream>
#include <algorithm>
#include <ostream>
#include <string>
#include <tuple>
#include <typeinfo>
#include <utility>
#include <vector>

#include <boost/sml.hpp>

namespace util {
namespace sml {
    /** Sender class. Can be used to send a command to the server.
     *  The receiver will acknowledge the command by calling Ack().
     *  \startuml
     *    Sender->Receiver  : Command()
     *    Sender<--Receiver : Ack()
     *  \enduml
     */
    class Logger {
    public:
        template<class SM, class TEvent>
        void log_process_event(const TEvent&) {
            std::printf("[%s][process_event] %s\n", boost::sml::aux::get_type_name<SM>(), boost::sml::aux::get_type_name<TEvent>());
        }

        template<class SM, class TGuard, class TEvent>
        void log_guard(const TGuard&, const TEvent&, bool result) {
            std::printf("[%s][guard] %s %s %s\n", boost::sml::aux::get_type_name<SM>(), boost::sml::aux::get_type_name<TGuard>(),
                boost::sml::aux::get_type_name<TEvent>(), (result ? "[OK]" : "[Reject]"));
        }

        template<class SM, class TAction, class TEvent>
        void log_action(const TAction&, const TEvent&) {
            std::printf("[%s][action] %s %s\n", boost::sml::aux::get_type_name<SM>(), boost::sml::aux::get_type_name<TAction>(),
                boost::sml::aux::get_type_name<TEvent>());
        }

        template<class SM, class TSrcState, class TDstState>
        void log_state_change(const TSrcState& src, const TDstState& dst) {
            std::printf("[%s][transition] %s -> %s\n", boost::sml::aux::get_type_name<SM>(), src.c_str(), dst.c_str());
        }
    };

    inline void do_indent(std::ostream& out, unsigned int indent) { out << std::string(indent, ' '); }

    bool state_initialized = false;  // NOLINT(misc-definitions-in-headers)
    std::vector<std::string> completed_submachines;  // NOLINT(misc-definitions-in-headers)

    /** allows for checking if the type is boost::sml::front::seq_
     * This type is used by sml when there are lists of actions.
     */
    template <class... Ts>
    struct is_seq_ : boost::sml::aux::false_type {};  // NOLINT(readability-identifier-naming)
    template <class... Ts>
    struct is_seq_<boost::sml::front::seq_<Ts...>> : boost::sml::aux::true_type {};  // NOLINT(readability-identifier-naming)

    /** allows for checking if the type is boost::sml::front::not_
     * This type is used by sml inside of guards, when the guard value is negated with !
     *
     * The partial specialization matches if the type passed in is boost::sml::front::not_, causing the struct to
     * inherit from boost::sml::aux::true_type, which gives it a member called "value" that is set to true.
     * If the type passed doesn't match boost::sml::front::not_, it'll match the generic is_not_ which inherits
     * from boost::sml::aux::false_type, giving it a member called "value" that is set to false.
     */
    template <class... Ts>
    struct is_not_ : boost::sml::aux::false_type {};  // NOLINT(readability-identifier-naming)
    template <class... Ts>
    struct is_not_<boost::sml::front::not_<Ts...>> : boost::sml::aux::true_type {};  // NOLINT(readability-identifier-naming)

    /** provides access to the template parameter type of an boost::sml::front::not_<T>
     */
    template <class T>
    struct strip_not_ {
        using type = T;
    };  // NOLINT(readability-identifier-naming)
    template <class T>
    struct strip_not_<boost::sml::front::not_<T>> {
        using type = T;
    };  // NOLINT(readability-identifier-naming)

    /** allows for checking if the type is boost::sml::front::and_
     * This type is used by sml inside of guards when two guard functions are combined with &&
     */
    template <class... Ts>
    struct is_and_ : boost::sml::aux::false_type {};  // NOLINT(readability-identifier-naming)
    template <class... Ts>
    struct is_and_<boost::sml::front::and_<Ts...>> : boost::sml::aux::true_type {};  // NOLINT(readability-identifier-naming)

    /** allows for checking if the type is boost::sml::front::or_
     * This type is used by sml inside of guards when two guard functions are combined with ||
     */
    template <class... Ts>
    struct is_or_ : boost::sml::aux::false_type {};  // NOLINT(readability-identifier-naming)
    template <class... Ts>
    struct is_or_<boost::sml::front::or_<Ts...>> : boost::sml::aux::true_type {};  // NOLINT(readability-identifier-naming)

    /** uses std::tuple_element and std::tuple to access the Nth type in a parameter pack
     */
    template <int N, class... Ts>
    using NthTypeOf = typename std::tuple_element<N, std::tuple<Ts...>>::type;

    /** gets the size of a parameter pack
     * this isn't really necessary, sizeof...(Ts) can be used directly instead
     */
    template <class... Ts>
    struct count {                                     // NOLINT(readability-identifier-naming)
        static const std::size_t value = sizeof...(Ts);  // NOLINT(readability-identifier-naming)
    };

    /** allows for checking if the type is boost::sml::aux::zero_wrapper
     * sml puts this around types inside of guards and event sequences
     */
    template <class T>
    struct is_zero_wrapper : boost::sml::aux::false_type {};  // NOLINT(readability-identifier-naming)
    template <class T>
    struct is_zero_wrapper<boost::sml::aux::zero_wrapper<T>> : boost::sml::aux::true_type {};  // NOLINT(readability-identifier-naming)

    /** if T is a zero wrapper, ::type will be the inner type. if not, it will be T.
     */
    template <class T>
    struct strip_zero_wrapper {
        using type = T;
    };  // NOLINT(readability-identifier-naming)
    template <class T>
    struct strip_zero_wrapper<boost::sml::aux::zero_wrapper<T>> {
        using type = T;
    };  // NOLINT(readability-identifier-naming)

    /** accesses the type of a state-machine, boost::sml::back::sm
     */
    template <class T>
    struct submachine_type {
        using type = T;
    };  // NOLINT(readability-identifier-naming)
    template <class T>
    struct submachine_type<boost::sml::back::sm<T>> {
        using type = typename T::sm;
    };  // NOLINT(readability-identifier-naming)

    /** print the types inside a boost::sml::front::seq_
     * These types came from a list of actions.
     */
    template <class... Ts>
    struct print_seq_types {  // NOLINT(readability-identifier-naming)
        template <int I>
        static void func(std::ostream& out) {
            constexpr auto param_pack_empty = (sizeof...(Ts) == I);
            if constexpr (!param_pack_empty) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
                using current_type = NthTypeOf<I, Ts...>;
                if constexpr (is_seq_<typename current_type::type>::value) {  // NOLINT(readability-braces-around-statements)
                    print_seq_types<typename current_type::type>::template func<0>(out);
                } else {  // NOLINT(readability-misleading-indentation)
                    out << boost::sml::aux::string<typename strip_zero_wrapper<current_type>::type>{}.c_str();
                }
                if constexpr (I + 1 < sizeof...(Ts)) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
                    out << ",\\n ";
                }
                print_seq_types<Ts...>::template func<I + 1>(out);
            }
        }
    };
    template <class... Ts>
    struct print_seq_types<boost::sml::front::seq_<Ts...>> {  // NOLINT(readability-identifier-naming)
        template <int I>
        static void func(std::ostream& out) {
            print_seq_types<Ts...>::template func<0>(out);
        }
    };

    /** print the types inside a guard
     * These can be a functor, an boost::sml::front::not_, an boost::sml::front::and_, or an boost::sml::front::or_ which makes
     * this one more complicated. They also involve the zero_wrapper.
     * The various partial specializations handle all of the possible types.
     */
    template <class... Ts>
    struct print_guard {  // NOLINT(readability-identifier-naming)
        template <int I>
        static void func(std::ostream& out, const std::string& sep = "") {
            constexpr auto param_pack_empty = (sizeof...(Ts) == I);
            if constexpr (!param_pack_empty) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
                using current_type = NthTypeOf<I, Ts...>;
                if constexpr (is_zero_wrapper<
                                    current_type>::value) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
                    print_guard<typename strip_zero_wrapper<current_type>::type>::template func<0>(out);
                } else {  // NOLINT(readability-misleading-indentation)
                    out << boost::sml::aux::string<current_type>{}.c_str();
                }

                if constexpr (I + 1 < sizeof...(Ts)) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
                    if (!sep.empty()) {
                        out << sep;
                    }
                }

                print_guard<Ts...>::template func<I + 1>(out, sep);
            }
        }
    };
    template <class T>
    struct print_guard<boost::sml::front::not_<T>> {  // NOLINT(readability-identifier-naming)
        template <int I>
        static void func(std::ostream& out, const std::string& /*sep*/ = "") {
            out << "!" << boost::sml::aux::string<typename strip_zero_wrapper<T>::type>{}.c_str();
        }
    };
    template <class... Ts>
    struct print_guard<boost::sml::front::and_<Ts...>> {  // NOLINT(readability-identifier-naming)
        template <int I>
        static void func(std::ostream& out, const std::string& /*sep*/ = "") {
            constexpr auto param_pack_empty = (sizeof...(Ts) == I);
            if constexpr (!param_pack_empty) {
                print_guard<Ts...>::template func<I>(out, " &&\\n ");
            }
        }
    };
    template <class... Ts>
    struct print_guard<boost::sml::front::or_<Ts...>> {  // NOLINT(readability-identifier-naming)
        template <int I>
        static void func(std::ostream& out, const std::string& /*sep*/ = "") {
            constexpr auto param_pack_empty = (sizeof...(Ts) == I);
            if constexpr (!param_pack_empty) {
                print_guard<Ts...>::template func<I>(out, " ||\\n ");
            }
        }
    };

    template <typename...>
    struct dump_transitions;

    template <int N, class T>
    void dump_transition(std::ostream& out) noexcept {
        constexpr auto src_is_sub_sm =
            !boost::sml::aux::is_same<boost::sml::aux::type_list<>, boost::sml::back::get_sub_sms<typename T::src_state>>::value;
        constexpr auto dst_is_sub_sm =
            !boost::sml::aux::is_same<boost::sml::aux::type_list<>, boost::sml::back::get_sub_sms<typename T::dst_state>>::value;

        std::string src_state, dst_state;
        if constexpr (src_is_sub_sm) {
            src_state = std::string{boost::sml::aux::string<typename submachine_type<typename T::src_state>::type>{}.c_str()};
        } else {  // NOLINT(readability-misleading-indentation)
            src_state = std::string{boost::sml::aux::string<typename T::src_state>{}.c_str()};
        }

        if constexpr (dst_is_sub_sm) {
            dst_state = std::string{boost::sml::aux::string<typename submachine_type<typename T::dst_state>::type>{}.c_str()};
        } else {  // NOLINT(readability-misleading-indentation)
            dst_state = std::string{boost::sml::aux::string<typename T::dst_state>{}.c_str()};
        }

        const auto dst_internal = boost::sml::aux::is_same<typename T::dst_state, boost::sml::front::internal>::value;

        const auto has_event = !boost::sml::aux::is_same<typename T::event, boost::sml::anonymous>::value;
        const auto has_guard = !boost::sml::aux::is_same<typename T::guard, boost::sml::front::always>::value;
        const auto has_action = !boost::sml::aux::is_same<typename T::action, boost::sml::front::none>::value;

        if (has_event && has_action && boost::sml::aux::is_same<typename T::action::type, boost::sml::front::actions::defer>::value) {
            do_indent(out, N);
            out << src_state << " : " << boost::sml::aux::get_type_name<typename T::event>() << " / defer\n";
            return;
        }

        if (dst_state == "terminate") {
            dst_state = "[*]";
        }

        if (T::initial) {
            if (state_initialized) {  // create an orthogonal section
                do_indent(out, N);
                out << "--\n";
            }

            state_initialized = true;
            do_indent(out, N);
            out << "[*] --> " << src_state << "\n";
        }

        if constexpr (src_is_sub_sm) {
            auto already_in =
                std::find(completed_submachines.begin(), completed_submachines.end(), src_state) != completed_submachines.end();
            if (!already_in) {
                completed_submachines.push_back(src_state);
                constexpr int indent = N + 2;
                do_indent(out, N);
                out << "state " << src_state << " {\n";
                bool prev_state = state_initialized;
                state_initialized = false;
                dump_transitions<typename T::src_state::transitions>::template func<indent>(out);
                do_indent(out, N);
                out << "}\n";
                state_initialized = prev_state;
            }
        }

        do_indent(out, N);
        out << src_state;
        if (!dst_internal) {
            out << " --> " << dst_state;
        }

        if (has_event || has_guard || has_action) {
            out << " :";
        }

        if (has_event) {
            out << " " << std::string{boost::sml::aux::string<typename T::event>{}.c_str()};
        }

        if (has_guard) {
            out << "\\n [";
            print_guard<typename T::guard::type>::template func<0>(out);
            out << "]";
        }

        if (has_action) {
            out << " \\n/ ";

            if constexpr (is_seq_<typename T::action::type>::value) {  // NOLINT(readability-braces-around-statements)
                out << "(";
                print_seq_types<typename T::action::type>::template func<0>(out);
                out << ")";
            } else {  // NOLINT(readability-misleading-indentation)
                out << boost::sml::aux::string<typename T::action::type>{}.c_str();
            }
        }

        out << "\n";

        if constexpr (dst_is_sub_sm) {
            auto already_in =
                std::find(completed_submachines.begin(), completed_submachines.end(), dst_state) != completed_submachines.end();
            if (!already_in) {
                completed_submachines.push_back(dst_state);
                constexpr int indent = N + 2;
                do_indent(out, N);
                out << "state " << dst_state << " {\n";
                bool prev_state = state_initialized;
                state_initialized = false;
                dump_transitions<typename T::dst_state::transitions>::template func<indent>(out);
                do_indent(out, N);
                out << "}\n";
                state_initialized = prev_state;
            }
        }
    }

    template <int INDENT, int I, class... Ts>
    void apply_dump_transition(std::ostream& out) {
    constexpr auto param_pack_empty = (sizeof...(Ts) == I);
        if constexpr (!param_pack_empty) {  // NOLINT(readability-braces-around-statements,bugprone-suspicious-semicolon)
            dump_transition<INDENT, NthTypeOf<I, Ts...>>(out);
            apply_dump_transition<INDENT, I + 1, Ts...>(out);
        }
    }

    template <typename...>
    struct dump_transitions {  // NOLINT(readability-identifier-naming)
        template <int INDENT>
        static void func(std::ostream&) {}
    };

    template <typename... Ts>
    struct dump_transitions<typename boost::sml::aux::type_list<Ts...>> {  // NOLINT(readability-identifier-naming)
    template <int INDENT>
    static void func(std::ostream& out) {
        apply_dump_transition<INDENT, 0, Ts...>(out);
    }
    };

    template <class T>
    void dump_uml(std::ostream& out) noexcept {
        out << "@startuml\n\n";
        dump_transitions<typename boost::sml::sm<T>::transitions>::template func<0>(out);
        out << "\n@enduml\n";
    }
}
}