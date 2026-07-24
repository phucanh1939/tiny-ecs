#pragma once

#include <utility>

namespace tinyecs
{
    template <typename TComponent>
    void World::addComponent(Entity entity, TComponent component)
    {
        if (!isValid(entity))
        {
            return;
        }

        ComponentStorage<TComponent> &storage = _componentStorages.getOrCreate<TComponent>();
        storage.add(entity, std::move(component));
    }

    template <typename TComponent>
    void World::removeComponent(Entity entity)
    {
        if (!isValid(entity))
        {
            return;
        }

        if (!_componentStorages.contains<TComponent>())
        {
            return;
        }

        ComponentStorage<TComponent> &storage = _componentStorages.get<TComponent>();
        storage.remove(entity);
    }

    template <typename... TComponents>
    Query<TComponents...> World::query()
    {
        return Query<TComponents...>(_componentStorages);
    }
}