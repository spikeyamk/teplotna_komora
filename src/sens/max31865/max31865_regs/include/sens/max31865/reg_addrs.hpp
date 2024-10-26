#pragma once

#include <array>
#include <bitset>

namespace sens {
namespace max31865 {
    struct RegAddrs {
        enum class RW {
            CONFIGURATION             = 0x80,
            HIGH_FAULT_THRESHOLD_MSBS = 0x83,
            HIGH_FAULT_THRESHOLD_LSBS = 0x84,
            LOW_FAULT_THRESHOLD_MSBS  = 0x85,
            LOW_FAULT_THRESHOLD_LSBS  = 0x86,
        };

        enum class RO {
            CONFIGURATION             = 0x00,
            RTD_MSBS                  = 0x01,
            RTD_LSBS                  = 0x02,
            HIGH_FAULT_THRESHOLD_MSBS = 0x03,
            HIGH_FAULT_THRESHOLD_LSBS = 0x04,
            LOW_FAULT_THRESHOLD_MSBS  = 0x05,
            LOW_FAULT_THRESHOLD_LSBS  = 0x06,
            FAULT_STATUS              = 0x07,
        };
    };
}
}