#pragma once

#include <vector>

#include <tinyecs/Entity.h>

namespace tinyecs
{
    class Chunk
    {
    public:
        static constexpr std::size_t Capacity = 128;

        bool full() const;

        std::uint32_t addEntity(const Entity& entity);

        Entity removeEntity(std::uint32_t index);

        Entity getEntity(std::uint32_t index) const;

        std::size_t entityCount() const;

    private:
        std::vector<Entity> _entities;
    };
}