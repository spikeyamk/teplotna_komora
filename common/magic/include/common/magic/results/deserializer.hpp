#pragma once

#include <serde/deserializer.hpp>

#include "common/magic/results/results.hpp"

namespace common {
namespace magic {
namespace results {
    using Deserializer = Pack::apply_to<Serde::Deserializer>;
}
}
}