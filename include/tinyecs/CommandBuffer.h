#pragma once

#include <cstddef>
#include <memory>
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

        template <typename... Components>
        void createEntity(Components &&...components);

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
        struct ICommand
        {
            virtual ~ICommand() = default;
            virtual void execute(World &world) = 0;
        };

        template <typename... Components>
        struct CreateEntityCommand;

        struct DestroyEntityCommand;

        template <typename T>
        struct AddComponentCommand;

        template <typename T>
        struct RemoveComponentCommand;

        std::vector<std::unique_ptr<ICommand>> _commands;
    };
}

#include <tinyecs/CommandBuffer.inl>
