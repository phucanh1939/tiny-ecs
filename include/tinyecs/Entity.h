#pragma once

#include <cstdint>

namespace tinyecs
{
    // Lightweight value that identifies an entity in a World.
    struct Entity
    {
        std::uint32_t id = 0;
        std::uint32_t version = 0;

        bool isValid() const
        {
            return version != 0;
        }
    };
}