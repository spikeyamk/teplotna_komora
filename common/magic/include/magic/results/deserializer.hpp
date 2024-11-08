#pragma once

#include <serde/deserializer.hpp>

#include "magic/results/results.hpp"

namespace magic {
namespace results {
    using Deserializer = Pack::apply_to<Serde::Deserializer>;
}
}