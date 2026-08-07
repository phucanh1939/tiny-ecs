#include <cassert>

#include <tinyecs/Chunk.h>

namespace tinyecs
{
    bool Chunk::full() const
    {
        return _entities.size() >= Chunk::Capacity;
    }

    std::uint32_t Chunk::addEntity(const Entity& entity)
    {
        assert(!full());

        _entities.push_back(entity);

        return static_cast<std::uint32_t>(_entities.size() - 1);
    }

    Entity Chunk::removeEntity(std::uint32_t index)
    {
        assert(index < _entities.size());

        const std::uint32_t lastIndex = static_cast<std::uint32_t>(_entities.size() - 1);

        if (index == lastIndex)
        {
            _entities.pop_back();
            return Entity{}; // return an invalid entity since no entity was moved
        }

        Entity movedEntity = _entities[lastIndex];

        _entities[index] = movedEntity;
        _entities.pop_back();

        return movedEntity;
    }

    Entity Chunk::getEntity(std::uint32_t index) const
    {
        assert(index < _entities.size());

        return _entities[index];
    }

    std::size_t Chunk::entityCount() const
    {
        return _entities.size();
    }
}