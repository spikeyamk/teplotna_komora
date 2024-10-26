#include <cassert>
#include "sens/max31865/fault_threshold.hpp"

namespace sens {
namespace max31865 {
    FaultThreshold::FaultThreshold(const ADC_Code high_fault_threshold, const ADC_Code low_fault_threshold) :
        high_fault_threshold { high_fault_threshold },
        low_fault_threshold { low_fault_threshold }
    {
        assert(
            (high_fault_threshold.value > low_fault_threshold.value)
            && (high_fault_threshold.value > low_fault_threshold.value + 1)
        );
    }

    FaultThreshold::FaultThreshold(const std::array<std::bitset<8>, 4>& serialized) :
        high_fault_threshold {
            ADC_Code { std::array<std::bitset<8>, 2> { serialized[0], serialized[1] } }
        },
        low_fault_threshold {
            ADC_Code { std::array<std::bitset<8>, 2> { serialized[2], serialized[3] } }
        }
    {}

    std::array<std::bitset<8>, 4> FaultThreshold::serialize() const {
        const auto high_fault_threshold_serialized { high_fault_threshold.serialize() };
        const auto low_fault_threshold_serialized { low_fault_threshold.serialize() };
        return {
            high_fault_threshold_serialized[0],
            high_fault_threshold_serialized[1],
            low_fault_threshold_serialized[0],
            low_fault_threshold_serialized[1]
        };
    }

    bool FaultThreshold::operator==(const FaultThreshold& other) const {
        return (
            (other.high_fault_threshold == high_fault_threshold)
            && (other.low_fault_threshold == low_fault_threshold)
        );
    }

    bool FaultThreshold::operator!=(const FaultThreshold& other) const {
        return !(*this == other);
    }
}
}