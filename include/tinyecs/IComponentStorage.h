#pragma once

namespace tinyecs
{
    class IComponentStorage
    {
    public:
        virtual ~IComponentStorage() = default;

        virtual void debugPrintMemoryLayout() const = 0;
    };
}
