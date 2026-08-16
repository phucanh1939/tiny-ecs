#pragma once

namespace tinyecs
{
    class Archetype;

    struct EntityLocation
    {
        Archetype *archetype = nullptr;
        std::uint32_t chunkIndex = 0;
        std::uint32_t entityIndex = 0;
    };
}
