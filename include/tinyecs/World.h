#pragma once

#include <cstdint>
#include <vector>

#include <tinyecs/ComponentStorageRegistry.h>
#include <tinyecs/Entity.h>
#include <tinyecs/Query.h>

namespace tinyecs
{
    // Owns the ECS state and provides the main API for managing entities and
    // their components.
    //
    // The World is responsible for:
    // - Creating and destroying entities.
    // - Adding and removing components from entities.
    // - Providing access to components.
    // - Creating queries for iterating over entities with specific components.
    //
    // The World does not own or update gameplay systems. Systems are defined and
    // controlled by the user, who decides when and in what order they are updated.
    class World
    {
    public:
        World();
        ~World();

        // World owns resources, so copying is disabled.
        // A shallow copy could make two World objects refer to the same resources,
        // causing double deletion or invalid references when one World is destroyed.
        World(const World&) = delete;
        World& operator=(const World&) = delete;

        // Move operations only transfer ownership of resources and do not perform
        // operations that can throw exceptions. noexcept tells the standard library
        // that moving is safe, allowing containers such as std::vector to prefer
        // moving over copying when relocating World objects.
        //
        // If the move constructor is not noexcept, std::vector may choose to copy
        // the objects instead when the type is copyable, depending on the standard
        // library implementation. Copying provides stronger exception safety during
        // reallocation. If the type is not copyable, std::vector must use the move
        // constructor.
        //
        // A move operation can throw if it performs an operation that may fail, such
        // as allocating new memory, or if it moves a member whose move constructor
        // can throw:
        //
        //     class Resource
        //     {
        //     public:
        //         Resource(Resource&& other)
        //             : _data(new int[other._size]) // Allocation may throw
        //         {
        //         }
        //
        //     private:
        //         int* _data;
        //         std::size_t _size;
        //     };
        //
        // In our case, moving World should only transfer ownership of internal
        // resources, so noexcept is appropriate.
        //
        // For example, when std::vector<World> grows and relocates its elements to a
        // new memory location, noexcept guarantees that it can safely move World
        // objects without worrying about a failed move operation.
        World(World&&) noexcept = default;
        World& operator=(World&&) noexcept = default;

        Entity createEntity();

        void destroyEntity(Entity entity);

        bool isValid(Entity entity) const;

        template<typename TComponent>
        void addComponent(Entity entity, TComponent component);

        template<typename TComponent>
        void removeComponent(Entity entity);

        template<typename... TComponents>
        Query<TComponents...> query();

        void print() const;

    private:
        // Stores the current version for each entity ID.
        std::vector<std::uint32_t> _versions;

        // Tracks whether each entity ID is currently alive.
        std::vector<bool> _alive;

        // Stores component storage grouped by component type.
        ComponentStorageRegistry _componentStorages;
    };
}

#include <tinyecs/World.inl>