#include <tinyecs/Chunk.h>

#include <cassert>
#include <iostream>
#include <cstring>

namespace tinyecs
{
    Chunk::Chunk(const std::vector<ColumnLayout>& layouts, std::size_t capacity)
        : _layouts(&layouts), _data(std::make_unique<std::byte[]>(Size)), _capacity(capacity)
    {

    }

    std::size_t Chunk::entityCount() const
    {
        return _entityCount;
    }

    bool Chunk::full() const
    {
        return _entityCount >= _capacity;
    }

    Entity Chunk::getEntity(std::uint32_t index) const
    {
        assert(index < _entityCount);

        const auto* entities = reinterpret_cast<const Entity*>(_data.get());
        return entities[index];
    }

    std::uint32_t Chunk::addEntity(const Entity& entity)
    {
        if (full())
            return InvalidIndex;

        auto* entities = reinterpret_cast<Entity*>(_data.get());
        const std::uint32_t index = static_cast<std::uint32_t>(_entityCount);

        entities[index] = entity;
        ++_entityCount;

        return index;
    }

    Entity Chunk::removeEntity(std::uint32_t index)
    {
        assert(index < _entityCount);

        auto* entities = reinterpret_cast<Entity*>(_data.get());
        const std::uint32_t lastIndex = static_cast<std::uint32_t>(_entityCount - 1);

        Entity moved = Entity{};

        if (index != lastIndex)
        {
            moved = entities[lastIndex];
            entities[index] = moved;
        }

        --_entityCount;

        return moved;
    }

    std::uint32_t Chunk::copyEntity(std::uint32_t sourceIndex, Chunk& destination)
    {
        assert(sourceIndex < _entityCount);

        const std::uint32_t destinationIndex = destination.addEntity(getEntity(sourceIndex));

        if (destinationIndex == InvalidIndex)
            return InvalidIndex;

        for (const ColumnLayout& sourceLayout : *_layouts)
        {
            const ColumnLayout* destinationLayout = destination.findColumnLayout(sourceLayout.type);

            if (destinationLayout == nullptr)
                continue;

            const void* source = static_cast<const std::byte*>(getColumnData(sourceLayout)) + sourceIndex * sourceLayout.elementSize;
            void* target = static_cast<std::byte*>(destination.getColumnData(*destinationLayout)) + destinationIndex * destinationLayout->elementSize;

            std::memcpy(target, source, sourceLayout.elementSize);
        }

        return destinationIndex;
    }

    void* Chunk::getComponent(ComponentType type, std::uint32_t index)
    {
        assert(index < _entityCount);

        const ColumnLayout* layout = findColumnLayout(type);

        if (layout == nullptr)
        {
            return nullptr;
        }

        return static_cast<std::byte*>(getColumnData(*layout)) + index * layout->elementSize;
    }

    const void* Chunk::getComponent(ComponentType type, std::uint32_t index) const
    {
        assert(index < _entityCount);

        const ColumnLayout* layout = findColumnLayout(type);

        if (layout == nullptr)
            return nullptr;

        return static_cast<const std::byte*>(getColumnData(*layout)) + index * layout->elementSize;
    }

    const ColumnLayout* Chunk::findColumnLayout(ComponentType type) const
    {
        for (const ColumnLayout& layout : *_layouts)
        {
            if (layout.type == type)
                return &layout;
        }

        return nullptr;
    }

    void* Chunk::getColumnData(const ColumnLayout& layout)
    {
        return _data.get() + layout.offset;
    }

    const void* Chunk::getColumnData(const ColumnLayout& layout) const
    {
        return _data.get() + layout.offset;
    }
}