#pragma once

#include <typeindex>
#include <unordered_map>

#include <tinyecs/ComponentInfo.h>

namespace tinyecs
{
    // Stores metadata for all component types.
    //
    // Maps C++ types to ECS component information:
    //
    //     Position
    //        |
    //        v
    //     ComponentInfo
    //          type = 0
    //          size = sizeof(Position)
    //
    class ComponentRegistry
    {
    public:
        template<typename T>
        ComponentInfo get();

    private:
        ComponentType _nextType = 0;

        std::unordered_map<std::type_index, ComponentInfo> _components;
    };


    template<typename T>
    ComponentInfo ComponentRegistry::get()
    {
        const std::type_index type = std::type_index(typeid(T));

        auto it = _components.find(type);

        if (it != _components.end())
        {
            return it->second;
        }

        ComponentInfo info
        {
            .type = _nextType++,
            .size = sizeof(T)
        };

        _components[type] = info;

        return info;
    }
}