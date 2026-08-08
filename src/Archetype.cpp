#include <tinyecs/Archetype.h>

namespace tinyecs
{
    Archetype::Archetype(ComponentSignature signature) : _signature(signature)
    {
    }

    std::size_t Archetype::entityCount() const
    {
        return 0;
    }

    EntityLocation Archetype::addEntity(const Entity& entity)
    {
        return EntityLocation{};
    }

    Entity Archetype::removeEntity(const EntityLocation& location)
    {
        return Entity{};
    }

    EntityLocation Archetype::copyEntity(const EntityLocation& source, Archetype& destination)
    {
        return EntityLocation{};
    }

    Entity Archetype::getEntity(const EntityLocation& location) const
    {
        return _chunks[location.chunkIndex].getEntity(location.entityIndex);
    }

    void* Archetype::getComponentMemory(ComponentType type, const EntityLocation& location)
    {
        return nullptr;
    }

    const void* Archetype::getComponentMemory(ComponentType type, const EntityLocation& location) const
    {
        return nullptr;
    }

    void* Archetype::getComponent(ComponentType type, const EntityLocation& location)
    {
        return _chunks[location.chunkIndex].getComponent(type, location.entityIndex);
    }

    const void* Archetype::getComponent(ComponentType type, const EntityLocation& location) const
    {
        return _chunks[location.chunkIndex].getComponent(type, location.entityIndex);
    }
}