#include <cassert>

#include <tinyecs/Query.h>
#include <tinyecs/ComponentRegistry.h>

namespace tinyecs
{
    template <typename... Components>
    Query<Components...>::Iterator::Iterator(const Query *query, std::size_t archetypeIndex, std::size_t chunkIndex, std::uint32_t entityIndex)
        : _query(query), _archetypeIndex(archetypeIndex), _chunkIndex(chunkIndex), _entityIndex(entityIndex)
    {
    }

    template <typename... Components>
    bool Query<Components...>::Iterator::operator==(const Iterator &other) const
    {
        return _query == other._query &&
               _archetypeIndex == other._archetypeIndex &&
               _chunkIndex == other._chunkIndex &&
               _entityIndex == other._entityIndex;
    }

    template <typename... Components>
    bool Query<Components...>::Iterator::operator!=(const Iterator &other) const
    {
        return !(*this == other);
    }

    template <typename... Components>
    typename Query<Components...>::Iterator &Query<Components...>::Iterator::operator++()
    {
        ++_entityIndex;

        // Next entity in the current chunk.
        Archetype *archetype = _query->_archetypes[_archetypeIndex];

        if (_entityIndex < archetype->chunkEntityCount(_chunkIndex))
            return *this;

        // Next chunk.
        ++_chunkIndex;
        _entityIndex = 0;

        if (_chunkIndex < archetype->chunkCount())
            return *this;

        // Next archetype.
        ++_archetypeIndex;
        _chunkIndex = 0;
        _entityIndex = 0;

        // If there are no more archetypes, this is end().
        return *this;
    }

    template <typename... Components>
    typename Query<Components...>::Result Query<Components...>::Iterator::operator*() const
    {
        assert(_query != nullptr);
        assert(_archetypeIndex < _query->_archetypes.size());

        Archetype *archetype = _query->_archetypes[_archetypeIndex];
        EntityLocation location{archetype, static_cast<std::uint32_t>(_chunkIndex), _entityIndex};

        Entity entity = archetype->getEntity(location);

        return Result{entity, *static_cast<Components *>(archetype->getComponent(ComponentRegistry::getComponentType<Components>(), location))...};
    }

    template <typename... Components>
    Query<Components...>::Query(std::vector<Archetype *> archetypes)
        : _archetypes(std::move(archetypes))
    {
    }

    template <typename... Components>
    typename Query<Components...>::Iterator Query<Components...>::begin()
    {
        // Find the first non-empty chunk and archetype
        for (std::size_t archetypeIndex = 0; archetypeIndex < _archetypes.size(); ++archetypeIndex)
        {
            Archetype *archetype = _archetypes[archetypeIndex];

            for (std::size_t chunkIndex = 0; chunkIndex < archetype->chunkCount(); ++chunkIndex)
            {
                if (archetype->chunkEntityCount(static_cast<std::uint32_t>(chunkIndex)) > 0)
                    return Iterator(this, archetypeIndex, chunkIndex, 0);
            }
        }

        return end();
    }

    template <typename... Components>
    typename Query<Components...>::Iterator Query<Components...>::end()
    {
        return Iterator(this, _archetypes.size(), 0, 0);
    }
}