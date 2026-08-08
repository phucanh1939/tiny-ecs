#include <cassert>

#include <tinyecs/CommandBuffer.h>

namespace tinyecs
{
    void CommandBuffer::destroyEntity(Entity entity)
    {
        assert(false && "Not implemented");
    }

    void CommandBuffer::playback(World& world)
    {
        assert(false && "Not implemented");
    }

    void CommandBuffer::clear()
    {
        assert(false && "Not implemented");
    }

    bool CommandBuffer::empty() const
    {
        return true;
    }

    std::size_t CommandBuffer::size() const
    {
        return 0;
    }
}