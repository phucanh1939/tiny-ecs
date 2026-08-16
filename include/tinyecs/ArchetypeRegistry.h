#pragma once

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

#include <tinyecs/Archetype.h>
#include <tinyecs/ComponentInfo.h>
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

        /// @brief Find archetype by signature
        /// @param signature 
        /// @return Archetype matched that signature, otherwise return nullptr
        Archetype* find(const ComponentSignature& signature);

        /// @brief Find archetype by signature
        /// @param signature 
        /// @return Archetype matched that signature, otherwise return nullptr
        const Archetype* find(const ComponentSignature& signature) const;

        /// @brief Get or create archetype from a signature
        /// @param signature 
        /// @param components DO WE EVEN NEED THIS??
        /// @return Archetype matched that signature
        Archetype& getOrCreate(const ComponentSignature& signature);

        /// @brief Get matched archetypes for a signature
        /// @param requiredSignature 
        /// @return 
        std::vector<Archetype*> getMatchedArchetypes(const ComponentSignature& requiredSignature) const;

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