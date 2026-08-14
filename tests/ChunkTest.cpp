#include "ChunkTest.h"

#include <cassert>
#include <cstring>
#include <vector>

#include <tinyecs/Chunk.h>
#include <tinyecs/ComponentRegistry.h>
#include <tinyecs/ColumnLayout.h>
#include <tinyecs/MemoryUtils.h>

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

        constexpr std::size_t TestCapacity = 128;

        std::vector<ColumnLayout> createPositionLayouts()
        {
            const ComponentType positionType =
                ComponentRegistry::getComponentType<Position>();

            return {
                ColumnLayout{
                    .type = positionType,
                    .offset = sizeof(Entity) * TestCapacity,
                    .elementSize = sizeof(Position),
                    .alignment = alignof(Position),
                },
            };
        }

        std::vector<ColumnLayout> createPositionVelocityLayouts()
        {
            const ComponentType positionType =
                ComponentRegistry::getComponentType<Position>();

            const ComponentType velocityType =
                ComponentRegistry::getComponentType<Velocity>();

            std::size_t offset = sizeof(Entity) * TestCapacity;

            offset = alignUp(offset, alignof(Position));

            const std::size_t positionOffset = offset;
            offset += sizeof(Position) * TestCapacity;

            offset = alignUp(offset, alignof(Velocity));

            const std::size_t velocityOffset = offset;

            return {
                ColumnLayout{
                    .type = positionType,
                    .offset = positionOffset,
                    .elementSize = sizeof(Position),
                    .alignment = alignof(Position),
                },
                ColumnLayout{
                    .type = velocityType,
                    .offset = velocityOffset,
                    .elementSize = sizeof(Velocity),
                    .alignment = alignof(Velocity),
                },
            };
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
        auto layouts = createPositionLayouts();
        Chunk chunk(layouts, TestCapacity);

        assert(chunk.entityCount() == 0);
        assert(!chunk.full());

        printPassed("Chunk::empty");
    }

    void ChunkTest::testAddEntity()
    {
        auto layouts = createPositionLayouts();
        Chunk chunk(layouts, TestCapacity);

        Entity entity{1, 0};

        const std::uint32_t index = chunk.addEntity(entity);

        assert(index == 0);
        assert(chunk.entityCount() == 1);
        assert(chunk.getEntity(0) == entity);

        printPassed("Chunk::addEntity");
    }

    void ChunkTest::testFull()
    {
        auto layouts = createPositionLayouts();
        Chunk chunk(layouts, TestCapacity);

        for (std::uint32_t i = 0; i < TestCapacity; ++i)
        {
            const std::uint32_t index = chunk.addEntity(Entity{i, 0});
            assert(index == i);
        }

        assert(chunk.entityCount() == TestCapacity);
        assert(chunk.full());

        const std::uint32_t index =
            chunk.addEntity(Entity{999, 0});

        assert(index == Chunk::InvalidIndex);
        assert(chunk.entityCount() == TestCapacity);

        printPassed("Chunk::full");
    }

    void ChunkTest::testGetEntity()
    {
        auto layouts = createPositionLayouts();
        Chunk chunk(layouts, TestCapacity);

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
        auto layouts = createPositionLayouts();

        const ComponentType positionType =
            ComponentRegistry::getComponentType<Position>();

        Chunk chunk(layouts, TestCapacity);

        chunk.addEntity(Entity{1, 0});

        Position position{10.0f, 20.0f};

        void* data = chunk.getComponent(positionType, 0);

        assert(data != nullptr);

        std::memcpy(data, &position, sizeof(Position));

        const auto* result =
            static_cast<const Position*>(
                chunk.getComponent(positionType, 0));

        assert(result->x == 10.0f);
        assert(result->y == 20.0f);

        const ComponentType velocityType =
            ComponentRegistry::getComponentType<Velocity>();

        assert(chunk.getComponent(velocityType, 0) == nullptr);

        printPassed("Chunk::getComponent");
    }

    void ChunkTest::testRemoveLast()
    {
        auto layouts = createPositionLayouts();
        Chunk chunk(layouts, TestCapacity);

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
        auto layouts = createPositionLayouts();
        Chunk chunk(layouts, TestCapacity);

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
        const ComponentType positionType =
            ComponentRegistry::getComponentType<Position>();

        const ComponentType velocityType =
            ComponentRegistry::getComponentType<Velocity>();

        auto sourceLayouts = createPositionVelocityLayouts();
        auto destinationLayouts = createPositionLayouts();

        Chunk source(sourceLayouts, TestCapacity);
        Chunk destination(destinationLayouts, TestCapacity);

        Entity entity{1, 0};

        source.addEntity(entity);

        Position position{10.0f, 20.0f};
        Velocity velocity{30.0f, 40.0f};

        std::memcpy(
            source.getComponent(positionType, 0),
            &position,
            sizeof(Position));

        std::memcpy(
            source.getComponent(velocityType, 0),
            &velocity,
            sizeof(Velocity));

        const std::uint32_t destinationIndex =
            source.copyEntity(0, destination);

        assert(destinationIndex == 0);
        assert(destination.entityCount() == 1);
        assert(destination.getEntity(0) == entity);

        const auto* copiedPosition =
            static_cast<const Position*>(
                destination.getComponent(positionType, 0));

        assert(copiedPosition->x == 10.0f);
        assert(copiedPosition->y == 20.0f);

        assert(destination.getComponent(velocityType, 0) == nullptr);

        printPassed("Chunk::copyEntity");
    }

    void ChunkTest::testCopyEntityToFullChunk()
    {
        auto layouts = createPositionLayouts();

        Chunk source(layouts, TestCapacity);
        Chunk destination(layouts, TestCapacity);

        source.addEntity(Entity{1, 0});

        for (std::uint32_t i = 0; i < TestCapacity; ++i)
            destination.addEntity(Entity{i, 0});

        const std::uint32_t index =
            source.copyEntity(0, destination);

        assert(index == Chunk::InvalidIndex);
        assert(destination.entityCount() == TestCapacity);

        printPassed("Chunk::copyEntity full destination");
    }
}