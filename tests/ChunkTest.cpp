#include "ChunkTest.h"

#include <cassert>
#include <cstring>

#include <tinyecs/Chunk.h>
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

        ComponentSignature createPositionSignature()
        {
            ComponentSignature signature;
            signature.add(ComponentRegistry::getComponentType<Position>());
            return signature;
        }

        ComponentSignature createPositionVelocitySignature()
        {
            ComponentSignature signature;
            signature.add(ComponentRegistry::getComponentType<Position>());
            signature.add(ComponentRegistry::getComponentType<Velocity>());
            return signature;
        }
    }

    void ChunkTest::run()
    {
        testEmpty();
        testAddEntity();
        testFull();
        testGetEntity();
        testGetComponent();
        testRemoveLast();
        testRemoveSwapBack();
        testCopyEntity();
        testCopyEntityToFullChunk();
    }

    void ChunkTest::testEmpty()
    {
        Chunk chunk(createPositionSignature());

        assert(chunk.entityCount() == 0);
        assert(!chunk.full());

        printPassed("Chunk::empty");
    }

    void ChunkTest::testAddEntity()
    {
        Chunk chunk(createPositionSignature());

        Entity entity{1, 0};

        const std::uint32_t index = chunk.addEntity(entity);

        assert(index == 0);
        assert(chunk.entityCount() == 1);
        assert(chunk.getEntity(0) == entity);

        printPassed("Chunk::addEntity");
    }

    void ChunkTest::testFull()
    {
        Chunk chunk(createPositionSignature());

        for (std::uint32_t i = 0; i < Chunk::Capacity; ++i)
        {
            const std::uint32_t index = chunk.addEntity(Entity{i, 0});
            assert(index == i);
        }

        assert(chunk.entityCount() == Chunk::Capacity);
        assert(chunk.full());

        const std::uint32_t index = chunk.addEntity(Entity{999, 0});

        assert(index == Chunk::InvalidIndex);
        assert(chunk.entityCount() == Chunk::Capacity);

        printPassed("Chunk::full");
    }

    void ChunkTest::testGetEntity()
    {
        Chunk chunk(createPositionSignature());

        Entity first{1, 0};
        Entity second{2, 0};

        chunk.addEntity(first);
        chunk.addEntity(second);

        assert(chunk.getEntity(0) == first);
        assert(chunk.getEntity(1) == second);

        printPassed("Chunk::getEntity");
    }

    void ChunkTest::testGetComponent()
    {
        ComponentSignature signature;

        const ComponentType positionType = ComponentRegistry::getComponentType<Position>();

        signature.add(positionType);

        Chunk chunk(signature);

        chunk.addEntity(Entity{1, 0});

        Position position{10.0f, 20.0f};

        void *data = chunk.getComponent(positionType, 0);

        assert(data != nullptr);

        std::memcpy(data, &position, sizeof(Position));

        const auto *result = static_cast<const Position *>(chunk.getComponent(positionType, 0));

        assert(result->x == 10.0f);
        assert(result->y == 20.0f);

        // Component not present.
        const ComponentType velocityType = ComponentRegistry::getComponentType<Velocity>();

        assert(chunk.getComponent(velocityType, 0) == nullptr);

        printPassed("Chunk::getComponent");
    }

    void ChunkTest::testRemoveLast()
    {
        Chunk chunk(createPositionSignature());

        Entity first{1, 0};
        Entity second{2, 0};

        chunk.addEntity(first);
        chunk.addEntity(second);

        Entity moved = chunk.removeEntity(1);

        assert(!moved.isValid());
        assert(chunk.entityCount() == 1);
        assert(chunk.getEntity(0) == first);

        printPassed("Chunk::removeEntity last");
    }

    void ChunkTest::testRemoveSwapBack()
    {
        Chunk chunk(createPositionSignature());

        Entity first{1, 0};
        Entity second{2, 0};
        Entity third{3, 0};

        chunk.addEntity(first);
        chunk.addEntity(second);
        chunk.addEntity(third);

        Entity moved = chunk.removeEntity(1);

        assert(moved == third);
        assert(chunk.entityCount() == 2);

        assert(chunk.getEntity(0) == first);
        assert(chunk.getEntity(1) == third);

        printPassed("Chunk::removeEntity swap-back");
    }

    void ChunkTest::testCopyEntity()
    {
        const ComponentType positionType = ComponentRegistry::getComponentType<Position>();
        const ComponentType velocityType = ComponentRegistry::getComponentType<Velocity>();

        ComponentSignature sourceSignature;
        sourceSignature.add(positionType);
        sourceSignature.add(velocityType);

        ComponentSignature destinationSignature;
        destinationSignature.add(positionType);

        Chunk source(sourceSignature);
        Chunk destination(destinationSignature);

        Entity entity{1, 0};

        source.addEntity(entity);

        Position position{10.0f, 20.0f};
        Velocity velocity{30.0f, 40.0f};

        std::memcpy(source.getComponent(positionType, 0),&position,sizeof(Position));
        std::memcpy(source.getComponent(velocityType, 0), &velocity, sizeof(Velocity));

        const std::uint32_t destinationIndex = source.copyEntity(0, destination);

        assert(destinationIndex == 0);
        assert(destination.entityCount() == 1);
        assert(destination.getEntity(0) == entity);

        const auto *copiedPosition =static_cast<const Position *>(destination.getComponent(positionType, 0));

        assert(copiedPosition->x == 10.0f);
        assert(copiedPosition->y == 20.0f);

        // Velocity isn't part of destination.
        assert(destination.getComponent(velocityType, 0) == nullptr);

        printPassed("Chunk::copyEntity");
    }

    void ChunkTest::testCopyEntityToFullChunk()
    {
        ComponentSignature signature = createPositionSignature();

        Chunk source(signature);
        Chunk destination(signature);

        source.addEntity(Entity{1, 0});

        for (std::uint32_t i = 0; i < Chunk::Capacity; ++i)
        {
            destination.addEntity(Entity{i, 0});
        }

        const std::uint32_t index = source.copyEntity(0, destination);

        assert(index == Chunk::InvalidIndex);
        assert(destination.entityCount() == Chunk::Capacity);

        printPassed("Chunk::copyEntity full destination");
    }
}