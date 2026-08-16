#pragma once

#include <cstddef>
#include <cstdint>
#include <tuple>
#include <vector>

#include <tinyecs/Archetype.h>
#include <tinyecs/Entity.h>

namespace tinyecs
{
template<typename... Components>
class Query
{
public:
    using Result = std::tuple<Entity, Components&...>;

    class Iterator
    {
        friend class Query;
    public:
        bool operator!=(const Iterator& other) const;
        bool operator==(const Iterator& other) const;
        Iterator& operator++();
        Result operator*() const;

    private:
        Iterator(const Query* query, std::size_t archetypeIndex, std::size_t chunkIndex, std::uint32_t entityIndex);
        const Query* _query = nullptr;
        std::size_t _archetypeIndex = 0;
        std::size_t _chunkIndex = 0;
        std::uint32_t _entityIndex = 0;
    };

    explicit Query(std::vector<Archetype*> archetypes);

    Iterator begin();
    Iterator end();

private:
    // Non-owning pointers to matching archetypes owned by the World.
    std::vector<Archetype*> _archetypes;
};
}

#include <tinyecs/Query.inl>