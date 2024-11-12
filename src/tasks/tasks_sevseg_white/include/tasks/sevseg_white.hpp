#pragma once

#include <iostream>
#include <optional>

#include "cmsis_os2.h"
#include "panel/sevseg/common/common.hpp"
#include "tasks/task.hpp"

namespace tasks {
    class SevsegWhite : public Task<SevsegWhite, 2 * 1024, "sevseg_white"> {
    private:
        std::optional<panel::sevseg::common::sevmap> sevmap { std::nullopt };
        StaticSemaphore_t sem_control_block;
        osSemaphoreId_t sem { nullptr };
        static constexpr uint32_t BLINK_TICKS = 1'000;
    private:
        SevsegWhite() = default;
    public:
        bool blinking { false };
        static SevsegWhite& get_instance();
        static void worker(void* arg);
        void push(const panel::sevseg::common::sevmap& in_sevmap);
    private:
        panel::sevseg::common::sevmap consume();
    };
}