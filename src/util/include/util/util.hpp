#pragma once

#include <cstdint>
#include <array>

#include "stm32f2xx_hal.h"
#include "iwdg.h"
#include "tim.h"

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

    inline void microsec_blocking_delay(const uint32_t us) {
        const uint32_t SYSTICK_LOAD { SystemCoreClock / 1000000U };
        const uint32_t SYSTICK_DELAY_CALIB { SYSTICK_LOAD >> 1 };

        do {
            const uint32_t start = SysTick->VAL;
            const uint32_t ticks = (us * SYSTICK_LOAD)-SYSTICK_DELAY_CALIB;
            while((start - SysTick->VAL) < ticks);
        } while (0);
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
}

namespace util {
    void turn_every_annoying_peripheral_off();
    void shutdown_endless_loop();
    HAL_StatusTypeDef twdg_init();
    inline void twdg_refresh() {
        __HAL_TIM_SET_COUNTER(&htim6, 0);
    }

    inline void iwdg_refresh() {
        HAL_IWDG_Refresh(&hiwdg);
    }
}