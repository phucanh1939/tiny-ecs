#include <tinyecs/Archetype.h>

namespace tinyecs
{
    Archetype::Archetype(ComponentSignature signature) : _signature(signature)
    {
    }

    std::size_t Archetype::entityCount() const
    {
        std::size_t count = 0;
        for (const Chunk &chunk : _chunks)
        {
            count += chunk.entityCount();
        }
        return count;
    }

    EntityLocation Archetype::addEntity(const Entity &entity)
    {
        std::uint32_t chunkIndex = findOrCreateChunk();
        Chunk &chunk = _chunks[chunkIndex];
        const std::uint32_t entityIndex = chunk.addEntity(entity);
        return EntityLocation{
            .archetype = this,
            .chunkIndex = chunkIndex,
            .entityIndex = entityIndex
        };
    }

    Entity Archetype::removeEntity(const EntityLocation &location)
    {
        Chunk &chunk = _chunks[location.chunkIndex];
        return chunk.removeEntity(location.entityIndex);
    }

    EntityLocation Archetype::copyEntity(const EntityLocation &source, Archetype &destination)
    {
        Chunk &chunk = _chunks[source.chunkIndex];
        const std::uint32_t destinationChunkIndex = destination.findOrCreateChunk();
        const std::uint32_t destinationEntityIndex = chunk.copyEntity(source.entityIndex, destination._chunks[destinationChunkIndex]);
        return EntityLocation{
            .archetype = &destination,
            .chunkIndex = destinationChunkIndex,
            .entityIndex = destinationEntityIndex,
        };
    }

    Entity Archetype::getEntity(const EntityLocation &location) const
    {
        return _chunks[location.chunkIndex].getEntity(location.entityIndex);
    }

    void *Archetype::getComponent(ComponentType type, const EntityLocation &location)
    {
        return _chunks[location.chunkIndex].getComponent(type, location.entityIndex);
    }

    const void *Archetype::getComponent(ComponentType type, const EntityLocation &location) const
    {
        return _chunks[location.chunkIndex].getComponent(type, location.entityIndex);
    }

    std::uint32_t Archetype::findOrCreateChunk()
    {
        for (std::size_t i = 0; i < _chunks.size(); ++i)
        {
            if (!_chunks[i].full())
            {
                return static_cast<std::uint32_t>(i);
            }
        }

        _chunks.emplace_back(_signature);

        return static_cast<std::uint32_t>(_chunks.size() - 1);
    }
}