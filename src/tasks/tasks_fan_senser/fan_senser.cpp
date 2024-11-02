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
    inline void init_test_fans() {}

    template<typename Registry, const size_t index = 0>
    requires (
        (Registry::array.size() == actu::fan::fb::fbs.size())
        && (index != Registry::array.size())
    )
    inline void init_test_fans() {
        if(actu::fan::fb::fbs[index].get().consume_rpm() <= 100.0f) {
            bksram::write_reset<Registry::array[index]>();
        }
        init_test_fans<Registry, index + 1>();
    }

    void FanSenser::worker(void* arg) {
        FanSenser& self { *static_cast<FanSenser*>(arg) };
        if(self.inited == false) {
            bksram::write_reset<bksram::ErrorCodes::FanSenser::Worker::INITED_FALSE>();
        }

        while(1) {
            init_test_fans<bksram::ErrorCodes::FanSenser::Worker::FanRegistry>();
            osDelay(1'000);
        }
    }

    void FanSenser::init() {
        if(actu::fan::fb::all::init() != HAL_OK) {
            bksram::write_reset<bksram::ErrorCodes::FanSenser::Init::FB_ALL>();
        }

        init_test_fans<bksram::ErrorCodes::FanSenser::Init::FanRegistry>();

        inited = true;
    }
}