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
        assert(false && "Not implemented");
    }

    void World::destroyEntity(Entity entity)
    {
        assert(false && "Not implemented");
    }

    bool World::isValid(Entity entity) const
    {
        return false;
    }
}