#include <tinyecs/Chunk.h>

namespace tinyecs
{
    Chunk::Chunk(const ComponentSignature& signature) : _signature(signature)
    {
    }

    std::size_t Chunk::entityCount() const
    {
        return 0;
    }

    bool Chunk::full() const
    {
        return false;
    }

    Entity Chunk::getEntity(std::uint32_t index) const
    {
        return Entity{};
    }

    std::uint32_t Chunk::addEntity(const Entity& entity)
    {
        return 0;
    }

    Entity Chunk::removeEntity(std::uint32_t index)
    {
        return Entity{};
    }

    std::uint32_t Chunk::copyEntity(std::uint32_t sourceIndex, Chunk& destination)
    {
        return 0;
    }

    void* Chunk::getComponentMemory(ComponentType type, std::uint32_t index)
    {
        return nullptr;
    }

    const void* Chunk::getComponentMemory(ComponentType type, std::uint32_t index) const
    {
        return nullptr;
    }

    void* Chunk::getComponent(ComponentType type, std::uint32_t index)
    {
        return nullptr;
    }

    const void* Chunk::getComponent(ComponentType type, std::uint32_t index) const
    {
        return nullptr;
    }
}