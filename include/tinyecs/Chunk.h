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

        static constexpr std::uint32_t InvalidIndex = std::numeric_limits<std::uint32_t>::max();

        explicit Chunk(const ComponentSignature& signature);

        std::size_t entityCount() const;

        bool full() const;

        Entity getEntity(std::uint32_t index) const;

        /// @brief Add new entity and setup empty slot for components
        /// @param entity 
        /// @return Index of the entity, Chunk::InvalidIndex if the chunk is full
        std::uint32_t addEntity(const Entity& entity);

        /// @brief Remove an entity and its component data
        /// @param index 
        /// @return Moved entity if swap-back, otherwise an invalid entity will be returned
        Entity removeEntity(std::uint32_t index);

        /// @brief Copies an entity and all component data shared by both chunks.
        /// @param sourceIndex 
        /// @param destination 
        /// @return Index of the entity in the destination chunk, Chunk::InvalidIndex if the destination is full
        std::uint32_t copyEntity(std::uint32_t sourceIndex, Chunk& destination);

        /// @brief Get pointer to component data
        /// @param type 
        /// @param index 
        /// @return Pointer to component data if the signature includes this component type, otherwise nullptr.
        void* getComponent(ComponentType type, std::uint32_t index);

        /// @brief Get pointer to component data
        /// @param type 
        /// @param index 
        /// @return Pointer to component data if the signature include this component type, otherwise return nullptr
        const void* getComponent(ComponentType type, std::uint32_t index) const;

    private:
        ComponentSignature _signature;
        std::vector<Entity> _entities;
        std::vector<ComponentColumn> _columns;

        ComponentColumn* getColumn(ComponentType type);
        const ComponentColumn* getColumn(ComponentType type) const;
    };
}