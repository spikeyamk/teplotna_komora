#pragma once 

#include "magic/results/deserializer.hpp"

namespace comm {
namespace rs232_uart {
    template<typename T>
    class TransmitterBase {
    public:
        using Derived = T;
        using CRTP = TransmitterBase<Derived>;
        using ResultVariant = magic::results::Deserializer::Variant;

        void operator()(const ResultVariant& result_variant) const {
            static_cast<Derived const&>(*this)(result_variant);
        }
    };
}
} 