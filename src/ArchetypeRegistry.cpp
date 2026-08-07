#include <tinyecs/ArchetypeRegistry.h>

namespace tinyecs
{
    ArchetypeRegistry::ArchetypeRegistry() : _root(nullptr)
    {
        Archetype& rootArchetype = getOrCreate(ComponentSignature{});
        _root = &rootArchetype;
    }

    Archetype* ArchetypeRegistry::find(const ComponentSignature& signature)
    {
        auto it = _archetypes.find(signature);
        if (it == _archetypes.end())
        {
            return nullptr;
        }
        return it->second.get();
    }

    const Archetype* ArchetypeRegistry::find(const ComponentSignature& signature) const
    {
        auto it = _archetypes.find(signature);

        if (it == _archetypes.end())
        {
            return nullptr;
        }

        return it->second.get();
    }


    Archetype& ArchetypeRegistry::getOrCreate(const ComponentSignature& signature)
    {
        auto it = _archetypes.find(signature);

        if (it != _archetypes.end())
        {
            return *it->second;
        }

        auto archetype = std::make_unique<Archetype>(signature);

        Archetype& reference = *archetype;

        _archetypes.emplace(signature, std::move(archetype));

        return reference;
    }


    Archetype& ArchetypeRegistry::root()
    {
        return *_root;
    }


    const Archetype& ArchetypeRegistry::root() const
    {
        return *_root;
    }


    std::size_t ArchetypeRegistry::count() const
    {
        return _archetypes.size();
    }
}
