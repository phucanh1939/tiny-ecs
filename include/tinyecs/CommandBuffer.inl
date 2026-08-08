#pragma once

#include <cassert>

#include <tinyecs/CommandBuffer.h>

namespace tinyecs
{
    template<typename T>
    void CommandBuffer::addComponent(Entity entity, const T& component)
    {
        assert(false && "Not implemented");
    }

    template<typename T>
    void CommandBuffer::removeComponent(Entity entity)
    {
        assert(false && "Not implemented");
    }
}