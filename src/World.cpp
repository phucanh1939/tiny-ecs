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

        return Entity{id, _versions[id]};
    }

    void World::destroyEntity(Entity entity)
    {
        if (!isValid(entity))
        {
            return;
        }

        _alive[entity.id] = false;
        ++_versions[entity.id];
    }

    bool World::isValid(Entity entity) const
    {
        if (!entity.isValid())
        {
            return false;
        }

        if (entity.id >= _versions.size())
        {
            return false;
        }

        return _alive[entity.id] && _versions[entity.id] == entity.version;
    }

    void World::print() const
    {
        std::cout << "entity_versions: [";

        for (std::size_t i = 0; i < _versions.size(); ++i)
        {
            if (i > 0)
            {
                std::cout << ", ";
            }

            std::cout << _versions[i];
        }

        std::cout << "]\n";

        std::cout << "entity_alives: [";

        for (std::size_t i = 0; i < _alive.size(); ++i)
        {
            if (i > 0)
            {
                std::cout << ", ";
            }

            std::cout << _alive[i];
        }

        std::cout << "]\n";

        _componentStorages.debugPrintMemoryLayout();
    }
}