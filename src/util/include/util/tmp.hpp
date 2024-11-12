#pragma once

#include <cstdint>
#include <algorithm>
#include <array>

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

    template <typename T, T First, T... Rest>
    struct is_unique : std::conditional_t<
        ((First != Rest) && ...),
        is_unique<T, Rest...>,
        std::false_type
    > {};

    template <typename T, T Last>
    struct is_unique<T, Last> : std::true_type {};

    template<typename T, T ... Args> requires is_unique<T, Args...>::value
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

    template<size_t N>
    class TemplateString {
    public:
        constexpr TemplateString(const char (&str)[N]) {
            std::copy_n(str, N, value);
        }
        
        char value[N];
    };

    template<size_t N>
    struct TemplateStringNonNullTerminated {
    public:
        constexpr TemplateStringNonNullTerminated(const char (&str)[N]) {
            std::copy_n(str, N - 1, value);
        }
        
        char value[N - 1];
    };
}