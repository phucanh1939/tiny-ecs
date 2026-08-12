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
        // Find an ID
        std::uint32_t id;
        if (!_freeEntityIds.empty())
        {
            id = _freeEntityIds.back();
            _freeEntityIds.pop_back();
        }
        else
        {
            id = static_cast<std::uint32_t>(_versions.size());
            _versions.push_back(1);
            _entityLocations.emplace_back();
        }

        // Create entity with the current version
        Entity entity{.id = id, .version = _versions[id]};

        // Add to root archetype
        Archetype &root = _archetypeRegistry.root();
        EntityLocation location = root.addEntity(entity);

        // Update entity location
        _entityLocations[id] = location;

        return entity;
    }

    void World::destroyEntity(Entity entity)
    {
        assert(isValid(entity));

        // Get the location
        EntityLocation location = _entityLocations[entity.id];

        // Remove the entity from its archetype.
        Entity movedEntity = location.archetype->removeEntity(location);

        // If another entity was moved into the removed slot,
        // update its location.
        if (movedEntity.isValid())
        {
            _entityLocations[movedEntity.id] = location;
        }

        // Invalidate old entity handles by changing its version.
        ++_versions[entity.id];

        // Allow the ID to be reused.
        _freeEntityIds.push_back(entity.id);
    }

    bool World::isValid(Entity entity) const
    {
        return entity.id < _versions.size() && _versions[entity.id] == entity.version;
        ;
    }
}