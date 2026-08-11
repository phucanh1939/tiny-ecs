#pragma once

#include <vector>

#include <tinyecs/Chunk.h>
#include <tinyecs/ComponentSignature.h>
#include <tinyecs/EntityLocation.h>

namespace tinyecs
{
    // Represents all entities that share the same component signature.
    class Archetype
    {
    public:
        explicit Archetype(ComponentSignature signature);

        const ComponentSignature& signature() const { return _signature; }

        std::size_t entityCount() const;

        /// @brief Add an entity to a chunk (one of chunks), return the location, component data will be empty slot
        /// @param entity 
        /// @return Added entity's location
        EntityLocation addEntity(const Entity& entity);

        /// @brief Remove an entity from a location
        /// @param location 
        /// @return Moved entity if swap-back, otherwise return an invalid entity
        Entity removeEntity(const EntityLocation& location);

        /// @brief Copy an entity and shared component data in this Archetype to another Archetype,
        /// @param source 
        /// @param destination 
        /// @return Coppied entity location in the destination Archetype
        EntityLocation copyEntity(const EntityLocation& source, Archetype& destination);

        /// @brief Get an entity from its location
        /// @param location 
        /// @return The entity from the location, if not found, return invalid one
        Entity getEntity(const EntityLocation& location) const;

        /// @brief Get a pointer to component data of an entity
        /// @param type 
        /// @param location 
        /// @return Pointer to component data if the entity and type is exist in this Archetype, otherwise return nullptr
        void* getComponent(ComponentType type, const EntityLocation& location);

        /// @brief Get a pointer to component data of an entity
        /// @param type 
        /// @param location 
        /// @return Pointer to component data if the entity and type is exist in this Archetype, otherwise return nullptr
        const void* getComponent(ComponentType type, const EntityLocation& location) const;

    private:
        ComponentSignature _signature;
        std::vector<Chunk> _chunks;

        std::uint32_t findOrCreateChunk();
    };
}