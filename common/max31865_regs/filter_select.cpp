#include "sens/max31865/filter_select.hpp"

namespace sens {
namespace max31865 {
    FilterSelect::FilterSelect(const std::bitset<8>& serialized) :
        filter_select { Masks::FilterSelect::Or((serialized & Masks::FilterSelect::AND).to_ulong()) }
    {}
}
}