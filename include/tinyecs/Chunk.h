#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <vector>

#include <tinyecs/ColumnLayout.h>
#include <tinyecs/ComponentSignature.h>
#include <tinyecs/Entity.h>

namespace tinyecs
{
    class Chunk
    {
    public:
        static constexpr std::size_t Size = 16 * 1024; // 16 KB
        static constexpr std::uint32_t InvalidIndex = std::numeric_limits<std::uint32_t>::max();

        Chunk(const std::vector<ColumnLayout>& layouts, std::size_t capacity);

        std::size_t entityCount() const;
        bool full() const;

        Entity getEntity(std::uint32_t index) const;

        /// @brief Add new entity and setup empty slot for components.
        /// @param entity
        /// @return Index of the entity, Chunk::InvalidIndex if the chunk is full.
        std::uint32_t addEntity(const Entity& entity);

        /// @brief Remove an entity and its component data.
        /// @param index
        /// @return Moved entity if swap-back, otherwise an invalid entity.
        Entity removeEntity(std::uint32_t index);

        /// @brief Copies an entity and all component data shared by both chunks.
        /// @param sourceIndex
        /// @param destination
        /// @return Index of the entity in the destination chunk, Chunk::InvalidIndex if the destination is full.
        std::uint32_t copyEntity(std::uint32_t sourceIndex, Chunk& destination);

        /// @brief Get pointer to component data.
        /// @param type
        /// @param index
        /// @return Pointer to component data if the component exists, otherwise nullptr.
        void* getComponent(ComponentType type, std::uint32_t index);

        /// @brief Get pointer to component data.
        /// @param type
        /// @param index
        /// @return Pointer to component data if the component exists, otherwise nullptr.
        const void* getComponent(ComponentType type, std::uint32_t index) const;

        /// @brief Get chunk capacity
        /// @return Chunk's capacity
        std::size_t capacity() const { return _capacity; }

    private:
        const std::vector<ColumnLayout>* _layouts;
        std::unique_ptr<std::byte[]> _data;
        std::size_t _entityCount = 0;
        std::size_t _capacity;

        const ColumnLayout* findColumnLayout(ComponentType type) const;
        void* getColumnData(const ColumnLayout& layout);
        const void* getColumnData(const ColumnLayout& layout) const;
    };
}
