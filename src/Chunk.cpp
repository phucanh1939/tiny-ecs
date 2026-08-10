#include <cassert>
#include <tinyecs/Chunk.h>
#include <tinyecs/ComponentRegistry.h>

namespace tinyecs
{
    Chunk::Chunk(const ComponentSignature &signature) : _signature(signature)
    {
        _entities.reserve(Capacity);
        for (ComponentType type : _signature)
        {
            _columns.emplace_back(ComponentRegistry::getComponentInfo(type));
        }
    }

    std::size_t Chunk::entityCount() const
    {
        return _entities.size();
    }

    bool Chunk::full() const
    {
        return entityCount() >= Chunk::Capacity;
    }

    Entity Chunk::getEntity(std::uint32_t index) const
    {
        assert(index < _entities.size());
        return _entities[index];
    }

    std::uint32_t Chunk::addEntity(const Entity &entity)
    {
        if (full())
            return Chunk::InvalidIndex;

        _entities.push_back(entity);

        for (ComponentColumn &column : _columns)
        {
            column.add();
        }

        return _entities.size() - 1;
    }

    Entity Chunk::removeEntity(std::uint32_t index)
    {
        assert(index < _entities.size());

        const std::uint32_t lastIndex = static_cast<std::uint32_t>(_entities.size() - 1);

        Entity movedEntity{};

        if (index != lastIndex)
        {
            movedEntity = _entities[lastIndex];
            _entities[index] = movedEntity;
        }

        _entities.pop_back();

        for (ComponentColumn &column : _columns)
        {
            column.remove(index);
        }

        return movedEntity;
    }

    std::uint32_t Chunk::copyEntity(std::uint32_t sourceIndex, Chunk &destination)
    {
        assert(sourceIndex < entityCount());

        // Copy entity
        const std::uint32_t destinationIndex = destination.addEntity(_entities[sourceIndex]);

        // Add failed? dest chunk full?
        if (destinationIndex == Chunk::InvalidIndex)
        {
            return Chunk::InvalidIndex;
        }

        // Copy shared components
        for (ComponentType type : _signature)
        {
            if (!destination._signature.contains(type))
            {
                continue;
            }

            const void *source = getComponent(type, sourceIndex);
            assert(source != nullptr);
            void *target = destination.getComponent(type, destinationIndex);
            assert(target != nullptr);
            const ComponentInfo &info = ComponentRegistry::getComponentInfo(type); // Already has component column, has the type info already, no need to look up it here
            std::memcpy(target, source, info.size);
        }

        return destinationIndex;
    }

    void *Chunk::getComponent(ComponentType type, std::uint32_t index)
    {
        ComponentColumn *column = getColumn(type);

        if (column == nullptr)
        {
            return nullptr;
        }

        return column->get(index);
    }

    const void *Chunk::getComponent(ComponentType type, std::uint32_t index) const
    {
        const ComponentColumn *column = getColumn(type);

        if (column == nullptr)
        {
            return nullptr;
        }

        return column->get(index);
    }

    ComponentColumn *Chunk::getColumn(ComponentType type)
    {
        for (ComponentColumn &column : _columns)
        {
            if (column.type() == type)
            {
                return &column;
            }
        }

        return nullptr;
    }

    const ComponentColumn *Chunk::getColumn(ComponentType type) const
    {
        for (const ComponentColumn &column : _columns)
        {
            if (column.type() == type)
            {
                return &column;
            }
        }

        return nullptr;
    }
}