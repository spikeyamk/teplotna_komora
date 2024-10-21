#pragma once

#include <array>

#include "cmsis_os.h"

namespace util {
       class RedirectStdout {
    private:
        bool threadsafe { false };
        static const uint32_t magic { 0xFEFE'FEF0 };

        std::array<uint32_t, 2048> task_buffer;
        StaticTask_t task_control_block;

        osMessageQueueId_t queue;
        StaticQueue_t queue_control_block;

        static constexpr uint32_t msg_count { 1024 };
        static constexpr uint32_t msg_size { 1 };
        std::array<uint32_t, msg_count * msg_size> queue_data_storage {};
    private:
        RedirectStdout() = default;
    public:
        bool init();
    private:
        static void worker(void* arg);
        static void transmit_storage(const std::array<uint32_t, msg_count * msg_size>& storage, size_t stopper);
    public:
        static void new_line();
        static RedirectStdout& get_instance();
        bool push(int ch);
        bool get_threadsafe() const;
        void turn_off_threadsafe();
        void lock();
    };
}