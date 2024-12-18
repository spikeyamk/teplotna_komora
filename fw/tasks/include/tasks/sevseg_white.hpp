#pragma once

#include <iostream>
#include <optional>

#include "cmsis_os2.h"
#include "panel/sevseg/common/common.hpp"
#include "tasks/task.hpp"

namespace tasks {
    class SevsegWhite : public TaskBase<SevsegWhite, 2 * 1024, "sevseg_white"> {
    private:
        std::optional<panel::sevseg::common::sevmap> sevmap { std::nullopt };
        StaticSemaphore_t mutex_control_block;
        osMutexId_t mutex { nullptr };
        static constexpr uint32_t BLINK_TICKS { 250 };
    private:
        SevsegWhite();
    public:
        bool blinking { false };
        static SevsegWhite& get_instance();
        static void worker(void* arg);
        void push(const panel::sevseg::common::sevmap& in_sevmap);
    private:
        panel::sevseg::common::sevmap consume();
    };
}