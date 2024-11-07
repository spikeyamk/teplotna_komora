#include "bksram/bksram.hpp"
#include "actu/fan/fb/fb.hpp"
#include "tasks/fan_senser.hpp"

namespace tasks {
    FanSenser& FanSenser::get_instance() {
        static FanSenser instance {};
        return instance;
    }

    template<typename Registry, const size_t index>
    requires (index == Registry::array.size())
    inline void check_fans_spinning() {}

    template<typename Registry, const size_t index = 0>
    requires (
        (Registry::array.size() == actu::fan::fb::fbs.size())
        && (index != Registry::array.size())
    )
    inline void check_fans_spinning() {
        if(actu::fan::fb::fbs[index].get().consume_rpm() <= 100.0f) {
            bksram::write_reset<Registry::array[index]>();
        }
        check_fans_spinning<Registry, index + 1>();
    }

    void FanSenser::worker(void* arg) {
        FanSenser& self { *static_cast<FanSenser*>(arg) };
        if(self.inited == false) {
            bksram::write_reset<bksram::ErrorCodes::FanSenser::Worker::INITED_FALSE>();
        }

        osDelay(5'000);

        while(1) {
            check_fans_spinning<bksram::ErrorCodes::FanSenser::Worker::FanRegistry>();
            osDelay(1'000);
        }
    }

    void FanSenser::init() {
        if(actu::fan::fb::all::init() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::FanSenser::Init::FB_ALL>();
        }

        osDelay(1'000);
        check_fans_spinning<bksram::ErrorCodes::FanSenser::Init::FanRegistry>();

        inited = true;
    }
}