#pragma once

#include <serde/serializer.hpp>

#include "common/magic/commands/commands.hpp"

namespace common {
namespace magic {
namespace commands {
    using Serializer = Pack::apply_to<Serde::Serializer>;
}
}
}