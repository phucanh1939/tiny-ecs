#include <tinyecs/ComponentStorageRegistry.h>

#include <iostream>

namespace tinyecs
{
    void ComponentStorageRegistry::debugPrintMemoryLayout() const
    {
        for (const auto &[type, storage] : _storages)
        {
            storage->print();
        }
    }
}