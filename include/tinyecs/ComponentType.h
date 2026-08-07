#pragma once

#include <cstdint>

namespace tinyecs
{
    // Uniquely identifies a component type within the ECS.
    //
    // ComponentType values are used to build ComponentSignatures, which in turn
    // identify archetypes. Each component type is assigned a unique integer ID
    // by the ECS.
    using ComponentType = std::uint32_t;
}
