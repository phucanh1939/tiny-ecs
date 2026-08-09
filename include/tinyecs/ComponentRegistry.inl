#pragma once

#include <typeindex>
#include <unordered_map>
#include <vector>

#include <tinyecs/ComponentRegistry.h>

namespace tinyecs
{
    template<typename T>
    ComponentType ComponentRegistry::getComponentType()
    {
        // typeid(T) return std::type_info
        // std::type_index is a wrapper of std::type_info provide needed hash function so it can be used as key in hashmap
        const std::type_index type = std::type_index(typeid(T));

        // check if this type already register
        auto it = _types.find(type);
        if (it != _types.end())
        {
            return it->second;
        }

        // new type, increase the _nextType after use as type id for this type
        ComponentInfo info
        {
            .type = _nextType++,
            .size = sizeof(T)
        };

        _types[type] = info.type;
        _infos.push_back(info);

        return info.type;
    }

    template<typename T>
    const ComponentInfo& ComponentRegistry::getComponentInfo()
    {
        return _infos[(getComponentType<T>())];
    }
}