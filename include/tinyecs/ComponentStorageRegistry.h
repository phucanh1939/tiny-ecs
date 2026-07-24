#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

#include <tinyecs/ComponentStorage.h>
#include <tinyecs/IComponentStorage.h>

namespace tinyecs
{
    class ComponentStorageRegistry
    {
    public:
        template<typename TComponent>
        ComponentStorage<TComponent>& getOrCreate();

        template<typename TComponent>
        ComponentStorage<TComponent>& get();

        template<typename TComponent>
        bool contains() const;

        void debugPrintMemoryLayout() const;
    private:
        std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>> _storages;
    };
}

#include <tinyecs/ComponentStorageRegistry.inl>