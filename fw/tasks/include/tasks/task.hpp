#pragma once

#include <array>
#include <string_view>
#include <cstring>
#include <algorithm>
#include <type_traits>
#include <utility>

#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "FreeRTOSConfig.h"
#include "util/tmp.hpp"

namespace tasks {
    template<size_t N>
    using TaskName = util::TemplateString<N>;

    template<typename Derived, const size_t stack_size, const TaskName t_task_name>
    requires (
        (stack_size >= configMINIMAL_STACK_SIZE)
        && (
            (sizeof(t_task_name.value) > 0)
            && (sizeof(t_task_name.value) <= configMAX_TASK_NAME_LEN)
        )
    )
    class TaskBase {
    protected:
        using CRTP = TaskBase<Derived, stack_size, t_task_name>;
    private:
        std::array<uint32_t, stack_size> stack_mem;
        StaticTask_t cb;
        const osThreadAttr_t attr {
            .name = t_task_name.value,
            .attr_bits = osThreadDetached,
            .cb_mem = &cb,
            .cb_size = sizeof(cb),
            .stack_mem = stack_mem.data(),
            .stack_size = stack_mem.size(),
            .priority = osPriorityNormal,
            .tz_module = 0,
            .reserved = 0,
        };
        osThreadId_t handle { nullptr };
    protected:
        TaskBase() = default;
    public:
        const std::string_view task_name { t_task_name.value };
    public:
        bool launch() {
            handle = osThreadNew(static_cast<Derived*>(this)->worker, static_cast<Derived*>(this), &attr);
            return handle != nullptr;
        }
    };
}