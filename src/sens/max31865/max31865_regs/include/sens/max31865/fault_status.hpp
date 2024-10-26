#pragma once

#include "sens/max31865/masks.hpp"

namespace sens {
namespace max31865 {
    class FaultStatus {
    public:
        Masks::FaultStatus::AlwaysActive::Or always_active; 
        Masks::FaultStatus::MasterInitiated::REFIN_MinusIsGreaterThanZeroPointEightFiveTimesVbias::Or master_initiated_refin_greater; 
        Masks::FaultStatus::MasterInitiated::REFIN_MinusIsLowerThanZeroPointEightFiveTimesVbiasForceMinusOpen::Or master_initiated_refin_lower; 
        Masks::FaultStatus::MasterInitiated::RTDIN_MinusIsLowerThanZeroPointEightFiveTimesVbiasForceMinusOpen::Or master_initiated_rtdin_lower; 
        Masks::FaultStatus::EveryConversion::Or every_conversion; 
    public:
        FaultStatus() = delete;
        FaultStatus(
            const Masks::FaultStatus::AlwaysActive::Or always_active,
            const Masks::FaultStatus::MasterInitiated::REFIN_MinusIsGreaterThanZeroPointEightFiveTimesVbias::Or master_initiated_refin_greater,
            const Masks::FaultStatus::MasterInitiated::REFIN_MinusIsLowerThanZeroPointEightFiveTimesVbiasForceMinusOpen::Or master_initiated_refin_lower,
            const Masks::FaultStatus::MasterInitiated::RTDIN_MinusIsLowerThanZeroPointEightFiveTimesVbiasForceMinusOpen::Or master_initiated_rtdin_lower,
            const Masks::FaultStatus::EveryConversion::Or every_conversion
        ) :
            always_active { always_active },
            master_initiated_refin_greater { master_initiated_refin_greater },
            master_initiated_refin_lower { master_initiated_refin_lower },
            master_initiated_rtdin_lower { master_initiated_rtdin_lower },
            every_conversion { every_conversion }
        {}
        FaultStatus(const std::bitset<8>& serialized);
        bool operator==(const FaultStatus& other) const;
        bool operator!=(const FaultStatus& other) const;
    };
}
}