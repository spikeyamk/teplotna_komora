#pragma once

#include <serde/deserializer.hpp>

#include "common/magic/commands/commands.hpp"

namespace common {
namespace magic {
namespace commands {
    using Deserializer = Pack::apply_to<Serde::Deserializer>;
}
}
}