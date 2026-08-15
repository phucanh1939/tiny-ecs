#pragma once

#include <tuple>
#include <utility>
#include <type_traits>

#include <tinyecs/World.h>

namespace tinyecs
{
    template <typename... Components>
    struct CommandBuffer::CreateEntityCommand : ICommand
    {
        std::tuple<std::decay_t<Components>...> components;

        explicit CreateEntityCommand(Components &&...components)
            : components(std::forward<Components>(components)...)
        {
        }

        void execute(World &world) override
        {
            std::apply(
                [&](auto &&...components)
                {
                    world.createEntity(std::forward<decltype(components)>(components)...);
                },
                std::move(components)
            );
        }
    };

    struct CommandBuffer::DestroyEntityCommand : ICommand
    {
        Entity entity;

        explicit DestroyEntityCommand(Entity entity)
            : entity(entity)
        {
        }

        void execute(World &world) override
        {
            world.destroyEntity(entity);
        }
    };

    template <typename T>
    struct CommandBuffer::AddComponentCommand : ICommand
    {
        Entity entity;
        T component;

        AddComponentCommand(Entity entity, const T &component)
            : entity(entity), component(component)
        {
        }

        void execute(World &world) override
        {
            world.addComponent(entity, component);
        }
    };

    template <typename T>
    struct CommandBuffer::RemoveComponentCommand : ICommand
    {
        Entity entity;

        explicit RemoveComponentCommand(Entity entity)
            : entity(entity)
        {
        }

        void execute(World &world) override
        {
            world.removeComponent<T>(entity);
        }
    };

    template <typename... Components>
    void CommandBuffer::createEntity(Components &&...components)
    {
        _commands.push_back(
            std::make_unique<CreateEntityCommand<Components...>>(
                std::forward<Components>(components)...));
    }

    template <typename T>
    void CommandBuffer::addComponent(Entity entity, const T &component)
    {
        _commands.push_back(
            std::make_unique<AddComponentCommand<T>>(entity, component));
    }

    template <typename T>
    void CommandBuffer::removeComponent(Entity entity)
    {
        _commands.push_back(
            std::make_unique<RemoveComponentCommand<T>>(entity));
    }
}