#pragma once

#include <memory>
#include <unordered_map>

#include <tinyecs/Archetype.h>
#include <tinyecs/ComponentSignature.h>
#include <tinyecs/ComponentSignatureHash.h>

namespace tinyecs
{
    // Owns all archetypes in the World.
    //
    // Invariants:
    // - Every unique ComponentSignature has exactly one Archetype.
    // - The empty signature {} always exists as the root archetype.
    class ArchetypeRegistry
    {
    public:
        ArchetypeRegistry();

        Archetype* find(const ComponentSignature& signature);

        const Archetype* find(const ComponentSignature& signature) const;

        Archetype& getOrCreate(const ComponentSignature& signature);

        Archetype& root();

        const Archetype& root() const;

        std::size_t count() const;

    private:
        std::unordered_map<ComponentSignature, std::unique_ptr<Archetype>> _archetypes;

        // Non-owning pointer.
        // The actual ownership is still inside _archetypes.
        Archetype* _root;
    };
}
