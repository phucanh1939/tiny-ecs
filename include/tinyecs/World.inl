#pragma once

#include <cassert>

#include <tinyecs/World.h>

namespace tinyecs
{
    template <typename T>
    T &World::getComponent(Entity entity)
    {
        assert(false && "Not implemented");
    }

    template <typename T>
    const T &World::getComponent(Entity entity) const
    {
        assert(false && "Not implemented");
    }

    template <typename T>
    void World::addComponent(Entity entity, const T &component)
    {
        assert(false && "Not implemented");
    }

    template <typename T>
    void World::removeComponent(Entity entity)
    {
        assert(false && "Not implemented");
    }

    template <typename... Components>
    Query<Components...> World::query()
    {
        assert(false && "Not implemented");
    }
}