#pragma once

#include <cstdint>
#include <vector>

#include <tinyecs/Entity.h>
#include <tinyecs/EntityLocation.h>
#include <tinyecs/ArchetypeRegistry.h>
#include <tinyecs/Query.h>

namespace tinyecs
{
    // Owns the ECS state.
    //
    // The World is responsible for:
    // - Creating and destroying entities.
    // - Managing archetypes.
    // - Managing component type registration.
    //
    // Every entity belongs to exactly one archetype.
    class World
    {
    public:
        World();
        ~World();

        World(const World &) = delete;
        World &operator=(const World &) = delete;

        World(World &&) noexcept = default;
        World &operator=(World &&) noexcept = default;

        Entity createEntity();
        
        void destroyEntity(Entity entity);
        
        bool isValid(Entity entity) const;

        template <typename T>
        T &getComponent(Entity entity);

        template <typename T>
        const T &getComponent(Entity entity) const;

        template <typename T>
        void addComponent(Entity entity, const T &component);

        template <typename T>
        void removeComponent(Entity entity);

        template<typename... Components>
        Query<Components...> query();

    private:
        // Track version of entity.
        //
        // _versions[i] is the current version of the entity with ID i.
        // When an entity is destroyed, its version increases so old handles
        // become invalid.
        std::vector<std::uint32_t> _versions;

        // Track destroyed entity IDs that can be reused.
        std::vector<std::uint32_t> _freeEntityIds;

        // Maps each entity ID to its current location.
        //
        // Used for fast archetype migration and removal.
        std::vector<EntityLocation> _entityLocations;

        // Owns every archetype in the world.
        ArchetypeRegistry _archetypeRegistry;
    };
}

#include <tinyecs/World.inl>
