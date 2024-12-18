#pragma once

#include <cstdint>
#include <algorithm>
#include <chrono>

#include "magic/commands/commands.hpp"
#include "magic/commands/serializer.hpp"
#include "magic/commands/deserializer.hpp"
#include "magic/results/results.hpp"
#include "magic/results/serializer.hpp"
#include "magic/results/deserializer.hpp"

namespace magic {
    static constexpr size_t MTU { std::max(commands::Pack::max_size(), results::Pack::max_size()) };
    static constexpr std::chrono::milliseconds DISCONNECT_TIMEOUT { 500 };
}