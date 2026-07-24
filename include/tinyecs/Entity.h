#pragma once

#include <cstdint>

namespace tinyecs
{
    // Lightweight handle that identifies an entity in a World.
    class Entity
    {
    public:
        Entity() = default;

        // A const member function cannot modify the Entity object.
        // The compiler reports an error if the function attempts to modify
        // any non-mutable member.
        bool isValid() const { return _version != 0; }

        std::uint32_t id() const { return _id; }
        std::uint32_t version() const { return _version; }

    private:
        // Only World can create an Entity with a specific ID and version.
        friend class World;

        Entity(std::uint32_t id, std::uint32_t version);

        std::uint32_t _id = 0;
        std::uint32_t _version = 0;
    };
}