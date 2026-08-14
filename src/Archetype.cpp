#include <tinyecs/Archetype.h>

#include <cassert>

#include <tinyecs/ComponentRegistry.h>
#include <tinyecs/MemoryUtils.h>

namespace tinyecs
{
    Archetype::Archetype(ComponentSignature signature) : _signature(signature)
    {
        calculateColumnLayouts();
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

    std::size_t Archetype::chunkCount() const
    {
        return _chunks.size();
    }

    std::size_t Archetype::chunkEntityCount(std::uint32_t chunkIndex) const
    {
        assert(chunkIndex < _chunks.size());
        return _chunks[chunkIndex].entityCount();
    }

    EntityLocation Archetype::addEntity(const Entity &entity)
    {
        std::uint32_t chunkIndex = findOrCreateChunk();
        Chunk &chunk = _chunks[chunkIndex];
        const std::uint32_t entityIndex = chunk.addEntity(entity);
        return EntityLocation{
            .archetype = this,
            .chunkIndex = chunkIndex,
            .entityIndex = entityIndex};
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

        _chunks.emplace_back(_columnLayouts, _chunkCapacity);

        return static_cast<std::uint32_t>(_chunks.size() - 1);
    }

    void Archetype::calculateColumnLayouts()
    {
        _chunkCapacity = 0;

        while (calculateChunkSize(_chunkCapacity + 1) <= Chunk::Size)
            ++_chunkCapacity;

        std::size_t offset = sizeof(Entity) * _chunkCapacity;

        for (ComponentType type : _signature)
        {
            const ComponentInfo &info = ComponentRegistry::getComponentInfo(type);

            offset = alignUp(offset, info.alignment);

            _columnLayouts.push_back(ColumnLayout{
                .type = type,
                .offset = offset,
                .elementSize = info.size,
                .alignment = info.alignment,
            });

            offset += info.size * _chunkCapacity;
        }
    }

    std::size_t Archetype::calculateChunkSize(std::size_t chunkCapacity) const
    {
        std::size_t offset = sizeof(Entity) * chunkCapacity;

        for (ComponentType type : _signature)
        {
            const ComponentInfo &info = ComponentRegistry::getComponentInfo(type);

            offset = alignUp(offset, info.alignment);
            offset += info.size * chunkCapacity;
        }

        return offset;
    }
}