#include <cassert>
#include <iostream>

#include <tinyecs/ArchetypeRegistry.h>
#include <tinyecs/ComponentSignature.h>

using namespace tinyecs;

void printPassed(const char* testName)
{
    std::cout << "\033[32m✓ " << testName << " passed\033[0m\n";
}

void testComponentSignature()
{
    ComponentSignature signature;

    assert(signature.empty());

    signature.add(0);

    assert(signature.contains(0));
    assert(!signature.contains(1));
    assert(!signature.empty());

    signature.remove(0);

    assert(!signature.contains(0));
    assert(signature.empty());

    printPassed("ComponentSignature");
}

void testArchetypeRegistryRoot()
{
    ArchetypeRegistry registry;

    // Registry must always contain root archetype.
    assert(registry.count() == 1);

    ComponentSignature empty;

    Archetype* root = registry.find(empty);

    assert(root != nullptr);
    assert(root->signature().empty());

    printPassed("Root archetype");
}

void testArchetypeRegistryCreateFind()
{
    ArchetypeRegistry registry;

    ComponentSignature signature;
    signature.add(0);

    Archetype& archetype = registry.getOrCreate(signature);

    Archetype* found = registry.find(signature);

    assert(found != nullptr);
    assert(found == &archetype);

    printPassed("Archetype lookup");
}

void testArchetypeRegistrySameSignature()
{
    ArchetypeRegistry registry;

    ComponentSignature signature;
    signature.add(0);
    signature.add(1);

    Archetype& first = registry.getOrCreate(signature);
    Archetype& second = registry.getOrCreate(signature);

    // Same signature must map to the same archetype.
    assert(&first == &second);

    // Still only root + one created archetype.
    assert(registry.count() == 2);

    printPassed("Archetype uniqueness");
}

void testChunkAddEntity()
{
    Chunk chunk;

    Entity entity{0, 1};

    std::uint32_t index = chunk.addEntity(entity);

    assert(index == 0);
    assert(chunk.entityCount() == 1);
    assert(chunk.getEntity(0) == entity);

    printPassed("Chunk add entity");
}

void testChunkRemoveLastEntity()
{
    Chunk chunk;

    Entity entity{0, 1};

    chunk.addEntity(entity);

    Entity moved = chunk.removeEntity(0);

    assert(!moved.isValid());
    assert(chunk.entityCount() == 0);

    printPassed("Chunk remove last entity");
}

void testChunkSwapBack()
{
    Chunk chunk;

    Entity first{0, 1};
    Entity second{1, 1};
    Entity third{2, 1};

    chunk.addEntity(first);
    chunk.addEntity(second);
    chunk.addEntity(third);

    Entity moved = chunk.removeEntity(1);

    assert(moved == third);

    assert(chunk.entityCount() == 2);
    assert(chunk.getEntity(0) == first);
    assert(chunk.getEntity(1) == third);

    printPassed("Chunk swap-back");
}

void testArchetypeAddEntity()
{
    Archetype archetype(ComponentSignature{});

    Entity entity{0, 1};

    EntityLocation location = archetype.addEntity(entity);

    assert(location.archetype == &archetype);
    assert(location.chunkIndex == 0);
    assert(location.entityIndex == 0);

    printPassed("Archetype add entity");
}

void testArchetypeRemoveLastEntity()
{
    Archetype archetype(ComponentSignature{});

    Entity first{0, 1};
    Entity second{1, 1};

    archetype.addEntity(first);
    EntityLocation secondLocation = archetype.addEntity(second);

    Entity moved = archetype.removeEntity(secondLocation);

    assert(!moved.isValid());

    printPassed("Archetype remove entity");
}

void testArchetypeSwapBack()
{
    Archetype archetype(ComponentSignature{});

    Entity first{0, 1};
    Entity second{1, 1};
    Entity third{2, 1};

    EntityLocation firstLocation = archetype.addEntity(first);
    EntityLocation secondLocation = archetype.addEntity(second);
    EntityLocation thirdLocation = archetype.addEntity(third);

    Entity moved = archetype.removeEntity(secondLocation);

    // The last entity should have been swapped into the removed slot.
    assert(moved == third);

    // The moved entity now occupies the removed entity's position.
    assert(archetype.getEntity(secondLocation) == third);

    printPassed("Archetype swap-back");
}

int main()
{
    testComponentSignature();

    testArchetypeRegistryRoot();
    testArchetypeRegistryCreateFind();
    testArchetypeRegistrySameSignature();

    testChunkAddEntity();
    testChunkRemoveLastEntity();
    testChunkSwapBack();

    testArchetypeAddEntity();
    testArchetypeRemoveLastEntity();
    testArchetypeSwapBack();

    std::cout << "\033[32m✓ All tests passed\033[0m\n";

    return 0;
}