#pragma once

#include <typeindex>
#include <unordered_map>
#include <vector>

#include <tinyecs/ComponentInfo.h>

namespace tinyecs
{
    // Registers component types and their metadata.
    //
    // Responsibilities:
    // - Assign a unique ComponentType to each C++ component type.
    // - Store metadata (size, type, etc.) for every registered component.
    //
    // Component registration is global for the entire application.
    // The same C++ component type always maps to the same ComponentType,
    // regardless of which World uses it.
    class ComponentRegistry
    {
    public:
        template<typename T>
        static ComponentType getComponentType();

        template<typename T>
        static const ComponentInfo& getComponentInfo();

        static const ComponentInfo& getComponentInfo(ComponentType type);

    private:
        inline static ComponentType _nextType = 0;

        inline static std::unordered_map<std::type_index, ComponentType> _types;

        // _infos[i] is the info of Component Type i
        inline static std::vector<ComponentInfo> _infos;
    };


    template<typename T>
    ComponentType ComponentRegistry::getComponentType()
    {
        const std::type_index type = std::type_index(typeid(T));

        auto it = _types.find(type);

        if (it != _types.end())
        {
            return it->second;
        }

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
        return getInfo(getComponentType<T>());
    }

    inline const ComponentInfo& ComponentRegistry::getComponentInfo(ComponentType type)
    {
        return _infos[type];
    }
}