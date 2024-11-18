#pragma once

#include <serde/serializer.hpp>

#include "magic/commands/commands.hpp"

namespace magic {
namespace commands {
    using Serializer = Pack::apply_to<Serde::Serializer>;
}
}