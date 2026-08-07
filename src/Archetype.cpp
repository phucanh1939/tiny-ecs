#include <utility>

#include <tinyecs/Archetype.h>

namespace tinyecs
{
    Archetype::Archetype(ComponentSignature signature) : _signature(std::move(signature))
    {
    }

    const ComponentSignature &Archetype::signature() const
    {
        return _signature;
    }

    EntityLocation Archetype::addEntity(const Entity &entity)
    {
        // Find a chunk with free space.
        for (std::uint32_t chunkIndex = 0; chunkIndex < _chunks.size(); ++chunkIndex)
        {
            if (_chunks[chunkIndex].full())
            {
                continue;
            }

            std::uint32_t entityIndex = _chunks[chunkIndex].addEntity(entity);

            return EntityLocation{this, chunkIndex, entityIndex};
        }

        // No chunk has space, create a new one.
        _chunks.emplace_back();

        Chunk &chunk = _chunks.back();

        std::uint32_t entityIndex = chunk.addEntity(entity);

        return EntityLocation{this, static_cast<std::uint32_t>(_chunks.size() - 1), entityIndex};
    }

    Entity Archetype::removeEntity(const EntityLocation &location)
    {
        return _chunks[location.chunkIndex].removeEntity(location.entityIndex);
    }

    Entity Archetype::getEntity(const EntityLocation &location) const
    {
        return _chunks[location.chunkIndex].getEntity(location.entityIndex);
    }
}