#include "sens/max31865/fault_status.hpp"

namespace sens {
namespace max31865 {
    FaultStatus::FaultStatus(const std::bitset<8>& serialized) :
        always_active {
            Masks::FaultStatus::AlwaysActive::Or(
                (
                    serialized & Masks::FaultStatus::AlwaysActive::AND
                ).to_ulong()
            )
        },
        master_initiated_refin_greater {
            Masks::FaultStatus::MasterInitiated::REFIN_MinusIsGreaterThanZeroPointEightFiveTimesVbias::Or(
                (
                    serialized & Masks::FaultStatus::MasterInitiated::REFIN_MinusIsGreaterThanZeroPointEightFiveTimesVbias::AND
                ).to_ulong()
            )
        },
        master_initiated_refin_lower {
            Masks::FaultStatus::MasterInitiated::REFIN_MinusIsLowerThanZeroPointEightFiveTimesVbiasForceMinusOpen::Or(
                (
                    serialized & Masks::FaultStatus::MasterInitiated::REFIN_MinusIsLowerThanZeroPointEightFiveTimesVbiasForceMinusOpen::AND
                ).to_ulong()
            )
        },
        master_initiated_rtdin_lower {
            Masks::FaultStatus::MasterInitiated::RTDIN_MinusIsLowerThanZeroPointEightFiveTimesVbiasForceMinusOpen::Or(
                (
                    serialized & Masks::FaultStatus::MasterInitiated::RTDIN_MinusIsLowerThanZeroPointEightFiveTimesVbiasForceMinusOpen::AND
                ).to_ulong()
            )
        },
        every_conversion {
            Masks::FaultStatus::EveryConversion::Or(
                (
                    serialized & Masks::FaultStatus::EveryConversion::AND
                ).to_ulong()
            )
        }
    {}

    bool FaultStatus::operator==(const FaultStatus& other) const {
        return (
            (always_active == other.always_active)
            && (master_initiated_refin_greater == other.master_initiated_refin_greater)
            && (master_initiated_refin_lower == other.master_initiated_refin_lower)
            && (master_initiated_rtdin_lower == other.master_initiated_rtdin_lower)
            && (every_conversion == other.every_conversion)
        );
    }

    bool FaultStatus::operator!=(const FaultStatus& other) const {
        return !(*this == other);
    }
}
}