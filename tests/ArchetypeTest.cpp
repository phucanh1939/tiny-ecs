#include "ArchetypeTest.h"

#include <cassert>

#include <tinyecs/Archetype.h>
#include <tinyecs/ComponentRegistry.h>

#include "TestUtils.h"

namespace tinyecs::test
{
    namespace
    {
        struct Position
        {
            float x;
            float y;
        };

        struct Velocity
        {
            float x;
            float y;
        };

        struct Health
        {
            int value;
        };
    }

    void ArchetypeTest::run()
    {
        testAddEntity();
        testRemoveEntityLast();
        testRemoveSwapback();
        testCopyEntity();
        testGetEntity();
        testGetComponent();
        testInvalidComponent();
        testChunkCount();
        testChunkEntityCount();
    }

    void ArchetypeTest::testAddEntity()
    {
        ComponentSignature signature;
        Archetype archetype(signature);

        Entity entity{1, 0};

        EntityLocation location = archetype.addEntity(entity);

        assert(archetype.entityCount() == 1);
        assert(archetype.getEntity(location) == entity);

        printPassed("Archetype::add entity");
    }

    void ArchetypeTest::testRemoveEntityLast()
    {
        ComponentSignature signature;
        Archetype archetype(signature);

        Entity first{1, 0};
        Entity second{2, 0};
        Entity third{3, 0};

        EntityLocation firstLocation = archetype.addEntity(first);
        EntityLocation secondLocation = archetype.addEntity(second);
        EntityLocation thirdLocation = archetype.addEntity(third);

        assert(archetype.entityCount() == 3);

        Entity moved = archetype.removeEntity(thirdLocation);

        assert(!moved.isValid());
        assert(archetype.entityCount() == 2);
        assert(archetype.getEntity(firstLocation) == first);
        assert(archetype.getEntity(secondLocation) == second);

        printPassed("Archetype::remove last entity");
    }

    void ArchetypeTest::testRemoveSwapback()
    {
        ComponentSignature signature;
        Archetype archetype(signature);

        Entity first{1, 0};
        Entity second{2, 0};
        Entity third{3, 0};

        EntityLocation firstLocation = archetype.addEntity(first);
        EntityLocation secondLocation = archetype.addEntity(second);
        EntityLocation thirdLocation = archetype.addEntity(third);

        Entity moved = archetype.removeEntity(secondLocation);

        assert(moved == third);
        assert(archetype.entityCount() == 2);
        assert(archetype.getEntity(firstLocation) == first);
        assert(archetype.getEntity(secondLocation) == third);

        printPassed("Archetype::remove swap-back");
    }

    void ArchetypeTest::testCopyEntity()
    {
        const ComponentType positionType = ComponentRegistry::getComponentType<Position>();
        const ComponentType velocityType = ComponentRegistry::getComponentType<Velocity>();
        const ComponentType healthType = ComponentRegistry::getComponentType<Health>();

        ComponentSignature sourceSignature;
        sourceSignature.add(positionType);
        sourceSignature.add(velocityType);

        ComponentSignature destinationSignature;
        destinationSignature.add(positionType);
        destinationSignature.add(velocityType);
        destinationSignature.add(healthType);

        Archetype source(sourceSignature);
        Archetype destination(destinationSignature);

        Entity first{1, 0};
        Entity second{2, 0};
        Entity third{3, 0};

        EntityLocation firstLocation = source.addEntity(first);
        EntityLocation secondLocation = source.addEntity(second);
        EntityLocation thirdLocation = source.addEntity(third);

        auto *sourcePosition = static_cast<Position *>(
            source.getComponent(positionType, secondLocation));

        auto *sourceVelocity = static_cast<Velocity *>(
            source.getComponent(velocityType, secondLocation));

        assert(sourcePosition != nullptr);
        assert(sourceVelocity != nullptr);

        sourcePosition->x = 10.0f;
        sourcePosition->y = 20.0f;

        sourceVelocity->x = 1.0f;
        sourceVelocity->y = 2.0f;

        EntityLocation destinationLocation =
            source.copyEntity(secondLocation, destination);

        assert(destination.entityCount() == 1);
        assert(destination.getEntity(destinationLocation) == second);

        auto *destinationPosition = static_cast<Position *>(
            destination.getComponent(positionType, destinationLocation));

        auto *destinationVelocity = static_cast<Velocity *>(
            destination.getComponent(velocityType, destinationLocation));

        auto *destinationHealth =
            destination.getComponent(healthType, destinationLocation);

        assert(destinationPosition != nullptr);
        assert(destinationVelocity != nullptr);
        assert(destinationHealth != nullptr);

        assert(destinationPosition->x == 10.0f);
        assert(destinationPosition->y == 20.0f);

        assert(destinationVelocity->x == 1.0f);
        assert(destinationVelocity->y == 2.0f);

        destinationPosition->x = 100.0f;

        assert(sourcePosition->x == 10.0f);

        printPassed("Archetype::copy entity");
    }

    void ArchetypeTest::testGetEntity()
    {
        ComponentSignature signature;
        Archetype archetype(signature);

        Entity first{1, 0};
        Entity second{2, 0};
        Entity third{3, 0};

        archetype.addEntity(first);
        EntityLocation secondLocation = archetype.addEntity(second);
        archetype.addEntity(third);

        Entity result = archetype.getEntity(secondLocation);

        assert(result == second);

        printPassed("Archetype::get entity");
    }

    void ArchetypeTest::testGetComponent()
    {
        const ComponentType positionType =
            ComponentRegistry::getComponentType<Position>();

        ComponentSignature signature;
        signature.add(positionType);

        Archetype archetype(signature);

        Entity first{1, 0};
        Entity second{2, 0};

        EntityLocation firstLocation = archetype.addEntity(first);
        EntityLocation secondLocation = archetype.addEntity(second);

        auto *firstPosition = static_cast<Position *>(
            archetype.getComponent(positionType, firstLocation));

        auto *secondPosition = static_cast<Position *>(
            archetype.getComponent(positionType, secondLocation));

        firstPosition->x = 10.0f;
        firstPosition->y = 20.0f;

        secondPosition->x = 30.0f;
        secondPosition->y = 40.0f;

        auto *result = static_cast<Position *>(
            archetype.getComponent(positionType, firstLocation));

        assert(result != nullptr);
        assert(result->x == 10.0f);
        assert(result->y == 20.0f);

        printPassed("Archetype::get component");
    }

    void ArchetypeTest::testInvalidComponent()
    {
        const ComponentType positionType =
            ComponentRegistry::getComponentType<Position>();

        const ComponentType velocityType =
            ComponentRegistry::getComponentType<Velocity>();

        ComponentSignature signature;
        signature.add(positionType);

        Archetype archetype(signature);

        Entity entity{1, 0};
        EntityLocation location = archetype.addEntity(entity);

        void *result = archetype.getComponent(velocityType, location);

        assert(result == nullptr);

        printPassed("Archetype::invalid component");
    }

    void ArchetypeTest::testChunkCount()
    {
        ComponentSignature signature;
        Archetype archetype(signature);

        assert(archetype.chunkCount() == 0);

        Entity entity{1, 0};
        archetype.addEntity(entity);

        assert(archetype.chunkCount() == 1);

        printPassed("Archetype::chunk count");
    }

    void ArchetypeTest::testChunkEntityCount()
    {
        ComponentSignature signature;
        Archetype archetype(signature);

        assert(archetype.chunkCount() == 0);

        Entity first{1, 0};
        Entity second{2, 0};

        archetype.addEntity(first);
        archetype.addEntity(second);

        assert(archetype.chunkCount() == 1);
        assert(archetype.chunkEntityCount(0) == 2);

        printPassed("Archetype::chunk entity count");
    }
}