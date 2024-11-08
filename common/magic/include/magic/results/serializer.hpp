#pragma once

#include <serde/serializer.hpp>

#include "magic/results/results.hpp"

namespace magic {
namespace results {
    using Serializer = Pack::apply_to<Serde::Serializer>;
}
}