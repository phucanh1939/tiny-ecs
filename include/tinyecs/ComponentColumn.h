#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include <tinyecs/ComponentInfo.h>

namespace tinyecs
{
    /// @brief Component data storage
    class ComponentColumn
    {
    public:
        explicit ComponentColumn(ComponentInfo info);

        ComponentType type() const;

        std::size_t count() const;

        /// @brief Get the pointer to component data of an entity at the index
        /// @param index 
        /// @return pointer to component data of an entity at the index
        void* get(std::uint32_t index);

        /// @brief Get the pointer to component data of an entity at the index
        /// @param index 
        /// @return pointer to component data of an entity at the index
        const void* get(std::uint32_t index) const;


        /// @brief Add an empty slot for one entity (at the end of _data)
        /// @return 
        std::uint32_t add();

        /// @brief Remove component data at index using swap-back
        /// @param index 
        void remove(std::uint32_t index);

    private:
        ComponentInfo _info;
        std::vector<std::byte> _data;
    };
}
