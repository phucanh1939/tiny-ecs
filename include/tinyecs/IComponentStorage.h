#pragma once

namespace tinyecs
{
    class IComponentStorage
    {
    public:
        virtual ~IComponentStorage() = default;

        virtual void print() const = 0;
    };
}
