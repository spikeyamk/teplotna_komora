#pragma once

#include <cstdint>
#include <algorithm>
#include <array>
#include <type_traits>

namespace util { 
    template<typename T>
    inline T& self_ref(void* arg) {
        T& ret { *reinterpret_cast<T*>(arg) };
        return ret;
    }

    template<typename T>
    inline const T& self_cref(void* arg) {
        const T& ret { *reinterpret_cast<const T*>(arg) };
        return ret;
    }

    template<typename T, T First, T... Rest>
    struct is_unique_by_value : std::conditional_t<
        ((First != Rest) && ...),
        is_unique_by_value<T, Rest...>,
        std::false_type
    > {};

    template<typename T, T Last>
    struct is_unique_by_value<T, Last> : std::true_type {};

    template<typename T, typename ... Rest>
    struct is_unique_by_type : std::conjunction<std::negation<std::is_same<T, Rest>>..., is_unique_by_type<Rest...>> {};

    template<typename T>
    struct is_unique_by_type<T> : std::true_type {};

    template<typename T, T ... Args> requires is_unique_by_value<T, Args...>::value
    class Registry {
    private:
        template<T key, T ... Keys>
        struct contains {};

        template<T key>
        struct contains<key> : std::false_type {};

        template<T key, T First, T... Rest>
        struct contains<key, First, Rest...> : std::conditional_t<
            (key == First),
            std::true_type,
            contains<key, Rest...>
        > {};
    public:
        static constexpr std::array<T, sizeof...(Args)> array { Args... };

        template<T key>
        static constexpr T get() {
            static_assert(contains<key, Args...>::value, "key not found");
            return key;
        }
    };

    template<size_t N, bool null_terminated = true>
    class TemplateString {
    public:
        char value[null_terminated ? N : (N - 1)];

        constexpr TemplateString(const char (&str)[N]) {
            std::copy_n(str, null_terminated ? N : (N - 1), value);
        }
    };

    template<size_t N>
    using TemplateStringNonNullTerminated = TemplateString<N, false>;

    template<TemplateStringNonNullTerminated template_name, auto template_value>
    requires std::is_enum_v<decltype(template_value)>
    struct EnumHolder {
        static constexpr decltype(template_value) value { template_value };
        static constexpr decltype(template_name) name { template_name };
    };
    
    template <typename T, typename = void>
    struct has_crtp : std::false_type {};

    template <typename T>
    struct has_crtp<T, std::void_t<typename T::CRTP>> : std::true_type {};
}