#pragma once

#include <cstdint>
#include <vector>

#include <tinyecs/ArchetypeRegistry.h>
#include <tinyecs/Entity.h>
#include <tinyecs/EntityLocation.h>

namespace tinyecs
{
    // Owns the ECS state.
    //
    // The World is responsible for:
    // - Creating and destroying entities.
    // - Managing archetypes.
    //
    // Every entity belongs to exactly one archetype.
    class World
    {
    public:
        World();
        ~World();

        World(const World&) = delete;
        World& operator=(const World&) = delete;

        World(World&&) noexcept = default;
        World& operator=(World&&) noexcept = default;

        Entity createEntity();

        void destroyEntity(Entity entity);

        bool isValid(Entity entity) const;

    private:
        // track version of entity
        // _versions[i] is the version of the entity with ID i.
        // If an entity does not match the version in _versions, it is considered invalid.
        std::vector<std::uint32_t> _versions;

        // Track free id for reuse
        std::vector<std::uint32_t> _freeEntityIds;

        // Owns every archetype in the world.
        ArchetypeRegistry _archetypes;

        // Maps each entity ID to its location in the world.
        std::vector<EntityLocation> _entityLocations;
    };
}