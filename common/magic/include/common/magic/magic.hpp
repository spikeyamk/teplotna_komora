#pragma once

#include <cstdint>

#include "common/magic/commands/commands.hpp"
#include "common/magic/commands/serializer.hpp"
#include "common/magic/commands/deserializer.hpp"
#include "common/magic/results/results.hpp"
#include "common/magic/results/serializer.hpp"
#include "common/magic/results/deserializer.hpp"

namespace common {
namespace magic {
    static constexpr size_t MTU { 9 };
    static constexpr uint32_t TIMEOUT_MS { 100 };
}
}