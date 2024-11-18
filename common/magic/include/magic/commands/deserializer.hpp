#pragma once

#include <serde/deserializer.hpp>

#include "magic/commands/commands.hpp"

namespace magic {
namespace commands {
    using Deserializer = Pack::apply_to<Serde::Deserializer>;
}
}