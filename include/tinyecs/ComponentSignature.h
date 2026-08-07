#pragma once

#include <bitset>
#include <cstddef>

#include <tinyecs/ComponentType.h>

namespace tinyecs
{
    // Maximum number of unique component types supported by the ECS.
    constexpr std::size_t MaxComponentTypes = 256;

    // Represents the set of component types that define an archetype.
    //
    // Each bit corresponds to a component type:
    //
    //     Position  -> bit 0
    //     Velocity  -> bit 1
    //     Health    -> bit 2
    //
    // For example:
    //
    //     {}                              Empty archetype
    //     {Position}
    //     {Position, Velocity}
    //
    // ComponentSignature is used as the unique key for looking up archetypes.
    class ComponentSignature
    {
    public:
        void add(ComponentType componentType);

        void remove(ComponentType componentType);

        bool contains(ComponentType componentType) const;

        bool empty() const;

        bool operator==(const ComponentSignature& other) const;

        const std::bitset<MaxComponentTypes>& bits() const;

    private:
        // All the member of this class is moveable, copyable, the compiler will generate the default copy/move constructor and assignment operator for us.
        // std::bitset is moveable and copyable
        std::bitset<MaxComponentTypes> _bits;
    };
}
