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

}