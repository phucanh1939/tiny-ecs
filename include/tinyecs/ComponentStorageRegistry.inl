#pragma once

#include <stdexcept>

#include <tinyecs/ComponentStorageRegistry.h>

namespace tinyecs
{
    template<typename TComponent>
    ComponentStorage<TComponent>& ComponentStorageRegistry::getOrCreate()
    {
        const std::type_index type = std::type_index(typeid(TComponent));
        auto iterator = _storages.find(type);

        if (iterator == _storages.end())
        {
            auto storage = std::make_unique<ComponentStorage<TComponent>>();
            ComponentStorage<TComponent>* storagePointer = storage.get();

            _storages.emplace(type, std::move(storage));

            return *storagePointer;
        }

        return *static_cast<ComponentStorage<TComponent>*>(iterator->second.get());
    }

    template<typename TComponent>
    ComponentStorage<TComponent>& ComponentStorageRegistry::get()
    {
        const std::type_index type = std::type_index(typeid(TComponent));
        auto iterator = _storages.find(type);

        if (iterator == _storages.end())
        {
            throw std::runtime_error("Component storage does not exist.");
        }

        return *static_cast<ComponentStorage<TComponent>*>(iterator->second.get());
    }

    template<typename TComponent>
    bool ComponentStorageRegistry::contains() const
    {
        const std::type_index type = std::type_index(typeid(TComponent));
        return _storages.contains(type);
    }
}