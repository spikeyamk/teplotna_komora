#pragma once

#include <cstdint>

#include "magic/commands/commands.hpp"
#include "magic/commands/serializer.hpp"
#include "magic/commands/deserializer.hpp"
#include "magic/results/results.hpp"
#include "magic/results/serializer.hpp"
#include "magic/results/deserializer.hpp"

namespace magic {
    static constexpr size_t MTU { 17 };
    static constexpr uint32_t TIMEOUT_MS { 100 };
}