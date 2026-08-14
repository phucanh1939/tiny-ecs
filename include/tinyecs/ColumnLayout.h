// ColumnLayout.h
#pragma once

#include <cstddef>

#include <tinyecs/ComponentType.h>

namespace tinyecs
{
    struct ColumnLayout
    {
        ComponentType type;      // Component type identifier.
        std::size_t offset;      // Byte offset from the beginning of the chunk memory block.
        std::size_t elementSize; // Size in bytes of one component element.
        std::size_t alignment;   // Alignment requirement of the component type.
    };
}
