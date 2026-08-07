#include <tinyecs/World.h>

namespace tinyecs
{
    World::World()
    {
        _archetypes.getOrCreate(ComponentSignature{});
    }

    World::~World()
    {
    }

    Entity World::createEntity()
    {
        std::uint32_t id;

        if (_freeEntityIds.empty())
        {
            // No reusable IDs, create a new one.
            id = static_cast<std::uint32_t>(_versions.size());

            // init the entity version to 1 (0 is invalid)
            _versions.push_back(1);

            Entity entity{id, _versions[id]};

            EntityLocation location = _archetypes.root().addEntity(entity);
            _entityLocations.push_back(location);

            return entity;
        }

        // Reuse a previously destroyed entity ID.
        id = _freeEntityIds.back();
        _freeEntityIds.pop_back();

        // version increase when entity is destroyed, no need to increase version here
        Entity entity{id, _versions[id]};

        EntityLocation location = _archetypes.root().addEntity(entity);
        _entityLocations[id] = location;

        return entity;
    }

    void World::destroyEntity(Entity entity)
    {
        if (!isValid(entity))
            return;

        // Get the location of the entity
        EntityLocation location = _entityLocations[entity.id];

        // Remove the entity from the archetype and get the moved entity (if any) - swap back
        Entity moved = location.archetype->removeEntity(location);

        // update destroyed entity location to default (unvalid location)
        _entityLocations[entity.id] = EntityLocation{};

        // If an entity was moved to fill the gap, update its location
        if (moved.isValid())
        {
            _entityLocations[moved.id] = location;
        }

        // Increment version for this id
        ++_versions[entity.id];

        // Mark the entity ID as free for reuse
        _freeEntityIds.push_back(entity.id);
    }

    bool World::isValid(Entity entity) const
    {
        if (!entity.isValid()) return false;
        if (entity.id >= _versions.size()) return false;
        return _versions[entity.id] == entity.version;
    }
}