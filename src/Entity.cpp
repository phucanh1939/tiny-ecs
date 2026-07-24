#include <tinyecs/Entity.h>

namespace tinyecs
{
    Entity::Entity(std::uint32_t id, std::uint32_t version)
        : _id(id),
          _version(version)
    {
    }

}