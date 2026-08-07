#pragma once

#include <tinyecs/ComponentSignature.h>
#include <tinyecs/Chunk.h>
#include <tinyecs/EntityLocation.h>

namespace tinyecs
{
    // Represents a group of entities that share the same component signature.
    //
    // Every archetype is uniquely identified by its ComponentSignature.
    // Multiple entities may belong to the same archetype, but there can only
    // be one archetype for a given signature within a World.
    //
    // In later milestones, the archetype will own the chunks that store the
    // component data for its entities.
    class Archetype
    {
    public:
        explicit Archetype(ComponentSignature signature);
        const ComponentSignature& signature() const;
        EntityLocation addEntity(const Entity& entity);
        Entity removeEntity(const EntityLocation& location);
        Entity getEntity(const EntityLocation& location) const;
    private:
        ComponentSignature _signature;
        std::vector<Chunk> _chunks;
    };
}