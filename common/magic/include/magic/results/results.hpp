#pragma once

#include <serde/common.hpp>

namespace magic {
namespace results {
    struct Connect {};
    struct Disconnect {};
    struct Nop {};

    struct WriteTemp {
        float value;
    };

    struct ReadTempCtl {
        uint16_t max31865_front;
        uint16_t max31865_rear;
        int16_t dac_front;
        int16_t dac_rear;
        uint16_t sht31_inside;
        uint16_t sht31_outside;
        int32_t p_front;
        int32_t i_front;
        int32_t d_front;
        int32_t p_rear;
        int32_t i_rear;
        int32_t d_rear;
    };

    using Pack = Serde::pack_holder<
        Connect,
        Disconnect,
        Nop,
        WriteTemp,
        ReadTempCtl
    >;
}
}