#pragma once

#include <vector>

#include <tinyecs/Chunk.h>
#include <tinyecs/ComponentSignature.h>
#include <tinyecs/EntityLocation.h>

namespace tinyecs
{
    // Represents all entities that share the same component signature.
    //
    // Responsibilities:
    // - Own chunks.
    // - Store entities and their component data.
    // - Move entities to another archetype.
    class Archetype
    {
    public:
        explicit Archetype(ComponentSignature signature);

        const ComponentSignature& signature() const { return _signature; }

        std::size_t entityCount() const;

        EntityLocation addEntity(const Entity& entity);

        Entity removeEntity(const EntityLocation& location);

        EntityLocation copyEntity(const EntityLocation& source, Archetype& destination);

        Entity getEntity(const EntityLocation& location) const;

        void* getComponentMemory(ComponentType type, const EntityLocation& location);

        const void* getComponentMemory(ComponentType type, const EntityLocation& location) const;

        void* getComponent(ComponentType type, const EntityLocation& location);

        const void* getComponent(ComponentType type, const EntityLocation& location) const;

    private:
        ComponentSignature _signature;

        std::vector<Chunk> _chunks;
    };
}