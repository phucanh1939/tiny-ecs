#pragma once

#include <cassert>

#include <tinyecs/ComponentRegistry.h>
#include "World.h"

namespace tinyecs
{

    template <typename... Components>
    Entity World::createEntity(Components &&...components)
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

        // Construct the signature
        ComponentSignature signature;
        (signature.add(ComponentRegistry::getComponentType<std::decay_t<Components>>()), ...);

        // Get or create archetype
        Archetype &archetype = _archetypeRegistry.getOrCreate(signature);

        // Add entity
        const EntityLocation location = archetype.addEntity(entity);

        // Update location
        _entityLocations[id] = location;

        // load component data
        auto initializeComponent = [&](const auto& component)
        {
            using Component = std::decay_t<decltype(component)>;

            ComponentType type = ComponentRegistry::getComponentType<Component>();
            void *destination = archetype.getComponent(type, location);

            std::memcpy(destination, &component, sizeof(Component));
        };
        (initializeComponent(components), ...);

        ++_entityCount;

        return entity;
    }

    template <typename T>
    void World::addComponent(Entity entity, const T &component)
    {
        assert(isValid(entity));

        // Get location -> source Archetype
        const EntityLocation sourceLocation = _entityLocations[entity.id];
        Archetype *sourceArchetype = sourceLocation.archetype;

        // Build destination signature
        ComponentSignature destinationSignature = sourceArchetype->signature();
        const ComponentType type = ComponentRegistry::getComponentType<T>();
        destinationSignature.add(type);

        // Get or create destination archetype
        Archetype &destination = _archetypeRegistry.getOrCreate(destinationSignature);

        // Copy entity and shared components data
        EntityLocation destinationLocation = sourceArchetype->copyEntity(sourceLocation, destination);

        // Set the newly added component's data
        void *destinationComponent = destination.getComponent(type, destinationLocation);

        assert(destinationComponent != nullptr);

        std::memcpy(destinationComponent, &component, sizeof(T));

        // Remove entity from source archetype
        Entity movedEntity = sourceArchetype->removeEntity(sourceLocation);

        // Update moved entity's location if swap-back happened
        if (movedEntity.isValid())
        {
            _entityLocations[movedEntity.id] = sourceLocation;
        }

        // Update entity's location
        _entityLocations[entity.id] = destinationLocation;
    }

    template <typename T>
    T &World::getComponent(Entity entity)
    {
        assert(isValid(entity));
        const EntityLocation &location = _entityLocations[entity.id];
        const ComponentType type = ComponentRegistry::getComponentType<T>();
        void *data = location.archetype->getComponent(type, location);
        assert(data != nullptr);
        return *static_cast<T *>(data);
    }

    template <typename T>
    const T &World::getComponent(Entity entity) const
    {
        assert(isValid(entity));
        const EntityLocation &location = _entityLocations[entity.id];
        const ComponentType type = ComponentRegistry::getComponentType<T>();
        const void *data = location.archetype->getComponent(type, location);
        assert(data != nullptr);
        return *static_cast<const T *>(data);
    }

    template <typename T>
    T *World::tryGetComponent(Entity entity)
    {
        if (!isValid(entity))
            return nullptr;

        const EntityLocation &location = _entityLocations[entity.id];
        const ComponentType type = ComponentRegistry::getComponentType<T>();
        return static_cast<T *>(location.archetype->getComponent(type, location));
    }

    template <typename T>
    const T *World::tryGetComponent(Entity entity) const
    {
        if (!isValid(entity))
            return nullptr;
        const EntityLocation &location = _entityLocations[entity.id];
        const ComponentType type = ComponentRegistry::getComponentType<T>();
        return static_cast<const T *>(location.archetype->getComponent(type, location));
    }

    template <typename T>
    void World::removeComponent(Entity entity)
    {
        assert(isValid(entity));

        // Get location and source archetype.
        const EntityLocation sourceLocation = _entityLocations[entity.id];
        Archetype *source = sourceLocation.archetype;

        // Build destination signature.
        const ComponentType type = ComponentRegistry::getComponentType<T>();
        ComponentSignature destinationSignature = source->signature();
        assert(destinationSignature.contains(type));
        destinationSignature.remove(type);

        // Get or create destination archetype.
        Archetype &destination = _archetypeRegistry.getOrCreate(destinationSignature);

        // Copy entity and shared components.
        const EntityLocation destinationLocation = source->copyEntity(sourceLocation, destination);

        // Remove entity from source archetype.
        const Entity movedEntity = source->removeEntity(sourceLocation);

        // Update moved entity's location if swap-back happened.
        if (movedEntity.isValid())
        {
            _entityLocations[movedEntity.id] = sourceLocation;
        }

        // Update the removed entity's location.
        _entityLocations[entity.id] = destinationLocation;
    }

    template <typename... Components>
    Query<Components...> World::query()
    {
        ComponentSignature requiredSignature;
        (requiredSignature.add(ComponentRegistry::getComponentType<Components>()), ...);
        return Query<Components...>(_archetypeRegistry.getMatchedArchetypes(requiredSignature));
    }
}