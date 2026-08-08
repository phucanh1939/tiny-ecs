#include <cassert>

#include <tinyecs/Query.h>

namespace tinyecs
{
    template <typename... Components>
    Query<Components...>::Iterator::Iterator()
    {
    }

    template <typename... Components>
    bool Query<Components...>::Iterator::operator!=(const Iterator &other) const
    {
        return false;
    }

    template <typename... Components>
    typename Query<Components...>::Iterator &
    Query<Components...>::Iterator::operator++()
    {
        assert(false && "Not implemented");
    }

    template <typename... Components>
    typename Query<Components...>::Result
    Query<Components...>::Iterator::operator*() const
    {
        assert(false && "Not implemented");
    }

    template <typename... Components>
    Query<Components...>::Query(std::vector<Archetype *> archetypes)
    {
    }

    template <typename... Components>
    typename Query<Components...>::Iterator
    Query<Components...>::begin()
    {
        assert(false && "Not implemented");
    }

    template <typename... Components>
    typename Query<Components...>::Iterator
    Query<Components...>::end()
    {
        assert(false && "Not implemented");
    }

    template <typename... Components>
    std::size_t Query<Components...>::count() const
    {
        return 0;
    }
}