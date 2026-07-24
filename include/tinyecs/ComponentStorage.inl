#pragma once

#include <stdexcept>
#include <utility>
#include <iostream>

namespace tinyecs
{
    template <typename TComponent>
    void ComponentStorage<TComponent>::add(Entity entity, TComponent component)
    {
        if (contains(entity))
        {
            throw std::runtime_error("Entity already has this component.");
        }

        const std::size_t index = _components.size();

        _components.push_back(std::move(component));
        _entities.push_back(entity);

        if (entity.id >= _entityToIndex.size())
        {
            _entityToIndex.resize(entity.id + 1, npos);
        }

        _entityToIndex[entity.id] = index;
    }

    template <typename TComponent>
    void ComponentStorage<TComponent>::remove(Entity entity)
    {
        if (!contains(entity))
        {
            return;
        }

        const std::size_t removedIndex = _entityToIndex[entity.id];
        const std::size_t lastIndex = _components.size() - 1;

        // Move the last component into the removed slot to keep storage dense.
        if (removedIndex != lastIndex)
        {
            _components[removedIndex] = std::move(_components[lastIndex]);
            _entities[removedIndex] = _entities[lastIndex];

            const Entity movedEntity = _entities[removedIndex];
            _entityToIndex[movedEntity.id] = removedIndex;
        }

        _components.pop_back();
        _entities.pop_back();

        _entityToIndex[entity.id] = npos;
    }

    template <typename TComponent>
    bool ComponentStorage<TComponent>::contains(Entity entity) const
    {
        if (entity.id >= _entityToIndex.size())
        {
            return false;
        }

        const std::size_t index = _entityToIndex[entity.id];
        return index != npos;
    }

    template <typename TComponent>
    TComponent &ComponentStorage<TComponent>::get(Entity entity)
    {
        if (!contains(entity))
        {
            throw std::runtime_error("Entity does not have this component.");
        }

        const std::size_t index = _entityToIndex[entity.id];
        return _components[index];
    }

    template <typename TComponent>
    const TComponent &ComponentStorage<TComponent>::get(Entity entity) const
    {
        if (!contains(entity))
        {
            throw std::runtime_error("Entity does not have this component.");
        }

        const std::size_t index = _entityToIndex[entity.id];
        return _components[index];
    }

    template <typename TComponent>
    void ComponentStorage<TComponent>::debugPrintMemoryLayout() const
    {
        std::cout << typeid(TComponent).name()
                  << "(size: "
                  << sizeof(TComponent)
                  << "): [";

        for (std::size_t i = 0; i < _components.size(); ++i)
        {
            if (i > 0)
            {
                std::cout << ", ";
            }

            std::cout << static_cast<const void *>(&_components[i]);
        }

        std::cout << "]\n";
    }
}