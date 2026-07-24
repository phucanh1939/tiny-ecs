#pragma once

#include <cstddef>
#include <vector>

#include <tinyecs/Entity.h>
#include <tinyecs/IComponentStorage.h>

namespace tinyecs
{
    template<typename TComponent>
    class ComponentStorage : public IComponentStorage
    {
    public:
        void add(Entity entity, TComponent component);

        void remove(Entity entity);

        bool contains(Entity entity) const;

        TComponent& get(Entity entity);

        const TComponent& get(Entity entity) const;

        void print() const override;
    private:
        // Components are stored densely.
        std::vector<TComponent> _components;

        // Stores which entity owns each component slot.
        std::vector<Entity> _entities;

        // Maps entity ID to component array index.
        std::vector<std::size_t> _entityToIndex;

        static constexpr std::size_t npos = static_cast<std::size_t>(-1);
    };
}

#include <tinyecs/ComponentStorage.inl>