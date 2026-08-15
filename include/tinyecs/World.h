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

        std::size_t entityCount() const { return _entityCount; }

        /// @brief Create an empty entity
        /// @return Created entity
        Entity createEntity();

        /// @brief Create an empty entity with components data
        /// @return Created entity
        template <typename... Components>
        Entity createEntity(Components&&... components);
        
        /// @brief Remove an entity & its components
        /// @param entity 
        void destroyEntity(Entity entity);
        
        /// @brief Check if an entity is valid or not
        /// @param entity 
        /// @return True if entity version is matched with latest version for entity.id
        bool isValid(Entity entity) const;

        /// @brief Add a component to an entity
        /// @tparam T Component Type
        /// @param entity 
        /// @param component 
        template <typename T>
        void addComponent(Entity entity, const T &component);

        /// @brief Get component data from an entity
        /// @tparam T Component type
        /// @param entity 
        /// @return Component data
        template <typename T>
        T &getComponent(Entity entity);

        /// @brief Get component data from an entity
        /// @tparam T Component type
        /// @param entity 
        /// @return Component data
        template <typename T>
        const T &getComponent(Entity entity) const;

        /// @brief Try Get component data from an entity
        /// @tparam T Component type
        /// @param entity 
        /// @return Component data pointer or null if component T not existed in entity
        template <typename T>
        T* tryGetComponent(Entity entity);
        
        /// @brief Try Get component data from an entity
        /// @tparam T Component type
        /// @param entity 
        /// @return Component data pointer or null if component T not existed in entity
        template <typename T>
        const T* tryGetComponent(Entity entity) const;

        /// @brief Remove a component from the entity
        /// @tparam T Component Type
        /// @param entity 
        template <typename T>
        void removeComponent(Entity entity);

        /// @brief Query to iterator through matched entity and components
        /// @tparam ...Components 
        /// @return 
        template<typename... Components>
        Query<Components...> query();

    private:
        std::size_t _entityCount = 0;

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
