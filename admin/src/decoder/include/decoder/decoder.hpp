#include "serde/deserializer.hpp"
# pragma once

namespace decoder {
    namespace request {
        struct ReadAll {
        };

        struct WriteTemperature {
            float temperature;
        };

        using ObjectSuperPack = pack_holder<Object, Super>;
        using ObjectSuperSerializer = ObjectSuperPack::apply_to<Serde::Serializer>;
        using ObjectSuperDeserializer = ObjectSuperPack::apply_to<Serde::Deserializer>;
    }

    namespace response {
        struct ReadAll {
            float temperatureSensor1;
            float temperatureSensor2;
        };

        struct WriteTemperature {
            bool success;
        };

        using ObjectSuperPack = pack_holder<Object, Super>;
        using ObjectSuperSerializer = ObjectSuperPack::apply_to<Serde::Serializer>;
        using ObjectSuperDeserializer = ObjectSuperPack::apply_to<Serde::Deserializer>;
    }
}
