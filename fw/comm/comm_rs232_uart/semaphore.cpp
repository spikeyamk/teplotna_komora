#include <cassert>
#include "comm/rs232_uart/semaphore.hpp"

namespace comm {
namespace rs232_uart {
    Semaphore::Semaphore() {
        const osSemaphoreAttr_t sem_attr {
            .name = "rs232_sem",
            .attr_bits = 0,
            .cb_mem = &sem_control_block,
            .cb_size = sizeof(sem_control_block),
        };
        sem = osSemaphoreNew(1, 0, &sem_attr);
        assert(sem != nullptr);
    }

    void Semaphore::release() {
        osSemaphoreRelease(sem);
    }

    void Semaphore::acquire() {
        osSemaphoreAcquire(sem, osWaitForever);
    }

    bool Semaphore::try_acquire_for(const std::chrono::milliseconds timeout) {
        return osSemaphoreAcquire(sem, static_cast<uint32_t>(timeout.count())) == osOK;
    }
}
}