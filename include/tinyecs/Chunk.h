#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include <tinyecs/ComponentSignature.h>
#include <tinyecs/ComponentColumn.h>
#include <tinyecs/Entity.h>

namespace tinyecs
{
    class Chunk
    {
    public:
        static constexpr std::size_t Capacity = 128;

        explicit Chunk(const ComponentSignature& signature);

        std::size_t entityCount() const;

        bool full() const;

        Entity getEntity(std::uint32_t index) const;

        /// @brief Add new entity and setup empty slot for components
        /// @param entity 
        /// @return Index of the entity
        std::uint32_t addEntity(const Entity& entity);

        /// @brief Remove an entity and its component data
        /// @param index 
        /// @return Moved entity if swap-back, otherwise an invalid entity will be returned
        Entity removeEntity(std::uint32_t index);

        /// @brief Copies an entity and all component data shared by both chunks.
        /// @param sourceIndex 
        /// @param destination 
        /// @return Index of the entity in the destination chunk
        std::uint32_t copyEntity(std::uint32_t sourceIndex, Chunk& destination);

        void* getComponentMemory(ComponentType type, std::uint32_t index);

        const void* getComponentMemory(ComponentType type, std::uint32_t index) const;

        void* getComponent(ComponentType type, std::uint32_t index);

        const void* getComponent(ComponentType type, std::uint32_t index) const;

    private:
        ComponentSignature _signature;
        std::vector<Entity> _entities;
        std::vector<ComponentColumn> _columns;
    };
}