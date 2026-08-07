#pragma once

#include <cstddef>

#include <tinyecs/ComponentType.h>

namespace tinyecs
{
    // Metadata describing a component type.
    //
    // ECS does not know the actual C++ component type.
    // It only needs information required for storage.
    //
    // Example:
    //
    // Position:
    //     type = 0
    //     size = 8
    //
    struct ComponentInfo
    {
        ComponentType type;
        std::size_t size;
    };
}
