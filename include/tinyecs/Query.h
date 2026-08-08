#pragma once

#include <cstddef>
#include <cstdint>
#include <tuple>
#include <vector>

#include <tinyecs/Archetype.h>

namespace tinyecs
{
template<typename... Components>
class Query
{
public:
    using Result = std::tuple<Entity, Components&...>;

    class Iterator
    {
    public:
        Iterator();

        bool operator!=(const Iterator& other) const;

        Iterator& operator++();

        Result operator*() const;

    private:
        const Query* _query = nullptr;
        std::size_t _archetypeIndex = 0;
        std::size_t _chunkIndex = 0;
        std::uint32_t _entityIndex = 0;
    };

    explicit Query(std::vector<Archetype*> archetypes);

    Iterator begin();
    Iterator end();

    std::size_t count() const;

private:
    std::vector<Archetype*> _archetypes;
};
}

#include <tinyecs/Query.inl>