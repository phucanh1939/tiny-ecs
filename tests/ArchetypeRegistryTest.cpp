#include "ArchetypeRegistryTest.h"

#include <cassert>

#include <tinyecs/ArchetypeRegistry.h>

#include "TestUtils.h"

namespace tinyecs::test
{
    void ArchetypeRegistryTest::run()
    {
        testCreate();
        testRoot();
        testFind();
        testGetOrCreate();
        testGetMatchedArchetypes();
    }

    void ArchetypeRegistryTest::testCreate()
    {
        ArchetypeRegistry registry;

        assert(registry.count() == 1);

        printPassed("ArchetypeRegistry::create");
    }

    void ArchetypeRegistryTest::testRoot()
    {
        ArchetypeRegistry registry;

        Archetype &root = registry.root();

        assert(root.signature().empty());

        printPassed("ArchetypeRegistry::root");
    }

    void ArchetypeRegistryTest::testFind()
    {
        ArchetypeRegistry registry;

        ComponentSignature signature;
        signature.add(0);

        assert(registry.find(signature) == nullptr);

        Archetype &created = registry.getOrCreate(signature);

        Archetype *found = registry.find(signature);

        assert(found != nullptr);
        assert(found == &created);

        printPassed("ArchetypeRegistry::find");
    }

    void ArchetypeRegistryTest::testGetOrCreate()
    {
        ArchetypeRegistry registry;

        ComponentSignature signature;
        signature.add(0);

        Archetype &first = registry.getOrCreate(signature);

        assert(registry.count() == 2);

        Archetype *found = registry.find(signature);

        assert(found == &first);

        Archetype &second = registry.getOrCreate(signature);

        assert(&first == &second);
        assert(registry.count() == 2);

        printPassed("ArchetypeRegistry::getOrCreate");
    }

    void ArchetypeRegistryTest::testGetMatchedArchetypes()
    {
        ArchetypeRegistry registry;

        ComponentSignature position;
        position.add(0);

        ComponentSignature positionVelocity;
        positionVelocity.add(0);
        positionVelocity.add(1);

        ComponentSignature health;
        health.add(2);

        Archetype &positionArchetype = registry.getOrCreate(position);
        Archetype &positionVelocityArchetype = registry.getOrCreate(positionVelocity);
        Archetype &healthArchetype = registry.getOrCreate(health);

        ComponentSignature required;
        required.add(0);

        std::vector<Archetype *> matched = registry.getMatchedArchetypes(required);

        assert(matched.size() == 2);

        bool foundPosition = false;
        bool foundPositionVelocity = false;

        for (Archetype *archetype : matched)
        {
            if (archetype == &positionArchetype)
                foundPosition = true;

            if (archetype == &positionVelocityArchetype)
                foundPositionVelocity = true;

            assert(archetype != &healthArchetype);
        }

        assert(foundPosition);
        assert(foundPositionVelocity);

        printPassed("ArchetypeRegistry::getMatchedArchetypes");
    }

}