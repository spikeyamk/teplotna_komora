#pragma once

#include <serde/serializer.hpp>

#include "common/magic/results/results.hpp"

namespace common {
namespace magic {
namespace results {
    using Serializer = Pack::apply_to<Serde::Serializer>;
}
}
}