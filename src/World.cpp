#include <tinyecs/World.h>

namespace tinyecs
{
    World::World()
    {
    }

    World::~World()
    {
    }

    Entity World::createEntity()
    {
        const std::uint32_t id = static_cast<std::uint32_t>(_versions.size());

        _versions.push_back(1);
        _alive.push_back(true);

        return Entity(id, _versions[id]);
    }

    void World::destroyEntity(Entity entity)
    {
        if (!isAlive(entity))
        {
            return;
        }

        _alive[entity.id()] = false;
        ++_versions[entity.id()];
    }

    bool World::isAlive(Entity entity) const
    {
        if (!entity.isValid())
        {
            return false;
        }

        if (entity.id() >= _versions.size())
        {
            return false;
        }

        return _alive[entity.id()] &&
               _versions[entity.id()] == entity.version();
    }

    template <typename TComponent>
    void World::addComponent(Entity entity, TComponent component)
    {
        // TODO: Implement component storage.
    }

    template <typename TComponent>
    void World::removeComponent(Entity entity)
    {
        // TODO: Implement component storage.
    }

    template <typename... TComponents>
    Query<TComponents...> World::query()
    {
        Test test;
        // TODO: Implement query creation.
        return {};
    }
}