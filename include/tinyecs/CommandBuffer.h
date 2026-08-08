#pragma once

#include <cstddef>
#include <vector>

#include <tinyecs/Entity.h>

namespace tinyecs
{
    class World;

    class CommandBuffer
    {
    public:
        CommandBuffer() = default;

        CommandBuffer(const CommandBuffer &) = delete;
        CommandBuffer &operator=(const CommandBuffer &) = delete;

        void destroyEntity(Entity entity);

        template <typename T>
        void addComponent(Entity entity, const T &component);

        template <typename T>
        void removeComponent(Entity entity);

        void playback(World &world);

        void clear();

        bool empty() const;
        std::size_t size() const;

    private:
        // Commands are stored here and executed during playback.
    };
}

#include <tinyecs/CommandBuffer.inl>
