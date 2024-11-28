#pragma once

#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "comm/rs232_uart/semaphore_base.hpp"

namespace comm {
namespace rs232_uart {
    class Semaphore : public comm::rs232_uart::SemaphoreBase<Semaphore> {
        friend CRTP;
    private:
        StaticSemaphore_t sem_control_block {};
        osSemaphoreId_t sem { nullptr };
    public:
        Semaphore();
    private:
        void release();
        void acquire();
        bool try_acquire_for(const uint32_t timeout_ms);
    };
}
}