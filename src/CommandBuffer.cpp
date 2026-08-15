#include <tinyecs/CommandBuffer.h>
#include <tinyecs/World.h>

namespace tinyecs
{
    void CommandBuffer::destroyEntity(Entity entity)
    {
        _commands.push_back(std::make_unique<DestroyEntityCommand>(entity));
    }

    void CommandBuffer::playback(World& world)
    {
        for (auto& command : _commands)
        {
            command->execute(world);
        }

        clear();
    }

    void CommandBuffer::clear()
    {
        _commands.clear();
    }

    bool CommandBuffer::empty() const
    {
        return _commands.empty();
    }

    std::size_t CommandBuffer::size() const
    {
        return _commands.size();
    }
}