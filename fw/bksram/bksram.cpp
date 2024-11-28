#include <iostream>
#include <algorithm>

#include "bksram/bksram.hpp"

namespace bksram {
    bool test() {
        const uint20_t read_err { read() };
        const auto find_it { std::find(ErrorCodes::Registry::array.begin(), ErrorCodes::Registry::array.end(), read_err) };
        if(find_it == ErrorCodes::Registry::array.end()) {
            return true;
        }

        std::printf("bksram::test(): read_err.unwrap(): 0x%lX\n", read_err.unwrap());
        return true;
    }
}