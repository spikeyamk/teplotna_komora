#include "CommandsHelper.hpp"

#include "common/magic/commands/Serializer.hpp"

SerializedCommand CommandsHelper::SerializeConnect() {
    const auto connect_serialized { common::magic::commands::Serializer::run(common::magic::commands::Connect()) };

    return SerializedCommand { reinterpret_cast<const char*>(connect_serialized.data()), connect_serialized.size() };
}

