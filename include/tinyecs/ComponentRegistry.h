#pragma once

#include <typeindex>
#include <unordered_map>
#include <vector>

#include <tinyecs/ComponentInfo.h>

namespace tinyecs
{
    /// @brief Global component registry, convert C++ type to Component Info
    class ComponentRegistry
    {
    public:
        template<typename T>
        static ComponentType getComponentType();

        template<typename T>
        static const ComponentInfo& getComponentInfo();

        static const ComponentInfo& getComponentInfo(ComponentType type) { return  _infos[type]; }

    private:
        // Inline static allows a single shared definition across translation units
        // If not inline, any file that use this header has each own static data (not shared)
        inline static ComponentType _nextType = 0;

        // typeid(T) return std::type_info
        // std::type_index is a wrapper of std::type_info provide needed hash function so it can be used as key in hashmap
        inline static std::unordered_map<std::type_index, ComponentType> _types;

        // _infos[i] is the info of Component Type i
        inline static std::vector<ComponentInfo> _infos;
    };
}

#include <tinyecs/ComponentRegistry.inl>
