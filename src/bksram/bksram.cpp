#include <algorithm>

#include "bksram/bksram.hpp"

namespace bksram {
    bool test() {
        const uint20_t read_err { read() };
        const auto find_it { std::find(ErrorCodes::Registry::array.begin(), ErrorCodes::Registry::array.end(), read_err) };
        if(
            (find_it != ErrorCodes::Registry::array.begin())
            && (find_it != ErrorCodes::Registry::array.end())
        ) {
            return false;
        }

        return true;
    }
}