#pragma once

#include <serde/common.hpp>

namespace magic {
namespace results {
    struct Connect {};
    struct Disconnect {};
    struct Nop {};

    struct ReadTempCtl {
        uint16_t max31865_front;
        uint16_t max31865_rear;
        int16_t dac_front;
        int16_t dac_rear;
        uint16_t sht31_inside;
        uint16_t sht31_outside;
        float p_front;
        float i_front;
        float d_front;
        float p_rear;
        float i_rear;
        float d_rear;
    };

    using Pack = Serde::pack_holder<
        Connect,
        Disconnect,
        Nop,
        ReadTempCtl
    >;
}
}