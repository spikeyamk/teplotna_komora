#pragma once

#include "sens/max31865/masks.hpp"

namespace sens {
namespace max31865 {
    class FilterSelect {
    public:
        Masks::FilterSelect::Or filter_select { Masks::FilterSelect::Or::SIXTY_HZ };
        FilterSelect() = default;
        FilterSelect(const std::bitset<8>& serialized);
    };
}
}