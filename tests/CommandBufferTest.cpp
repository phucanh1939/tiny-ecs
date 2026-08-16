#include "CommandBufferTest.h"

#include <cassert>

#include <tinyecs/CommandBuffer.h>
#include <tinyecs/World.h>

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

    void CommandBufferTest::run()
    {
        testCreateEntity();
        testDestroyEntity();
        testAddComponent();
        testRemoveComponent();
        testPlaybackOrder();
        testClear();
        testEmpty();
    }

    void CommandBufferTest::testCreateEntity()
    {
        World world;
        CommandBuffer commands;

        commands.createEntity(
            Position{10.0f, 20.0f},
            Velocity{1.0f, 2.0f},
            Health{100});

        // Recording a command must not modify the world.
        assert(commands.size() == 1);
        assert(!commands.empty());

        Entity entity{0, 1};
        assert(!world.isValid(entity));

        commands.playback(world);

        assert(commands.empty());
        assert(world.isValid(entity));

        const Position& position = world.getComponent<Position>(entity);
        const Velocity& velocity = world.getComponent<Velocity>(entity);
        const Health& health = world.getComponent<Health>(entity);

        assert(position.x == 10.0f);
        assert(position.y == 20.0f);

        assert(velocity.x == 1.0f);
        assert(velocity.y == 2.0f);

        assert(health.value == 100);

        printPassed("CommandBuffer::create entity");
    }

    void CommandBufferTest::testDestroyEntity()
    {
        World world;
        CommandBuffer commands;

        Entity entity = world.createEntity();

        assert(world.isValid(entity));

        commands.destroyEntity(entity);

        // Nothing happens until playback.
        assert(world.isValid(entity));
        assert(commands.size() == 1);

        commands.playback(world);

        assert(!world.isValid(entity));
        assert(commands.empty());

        printPassed("CommandBuffer::destroy entity");
    }

    void CommandBufferTest::testAddComponent()
    {
        World world;
        CommandBuffer commands;

        Entity entity = world.createEntity();

        commands.addComponent(entity, Position{10.0f, 20.0f});

        // Component doesn't exist until playback.
        assert(world.tryGetComponent<Position>(entity) == nullptr);

        commands.playback(world);

        const Position& position = world.getComponent<Position>(entity);

        assert(position.x == 10.0f);
        assert(position.y == 20.0f);

        printPassed("CommandBuffer::add component");
    }

    void CommandBufferTest::testRemoveComponent()
    {
        World world;
        CommandBuffer commands;

        Entity entity = world.createEntity(
            Position{10.0f, 20.0f},
            Velocity{1.0f, 2.0f});

        assert(world.tryGetComponent<Velocity>(entity) != nullptr);

        commands.removeComponent<Velocity>(entity);

        // Still exists before playback.
        assert(world.tryGetComponent<Velocity>(entity) != nullptr);

        commands.playback(world);

        assert(world.tryGetComponent<Velocity>(entity) == nullptr);
        assert(world.tryGetComponent<Position>(entity) != nullptr);

        const Position& position = world.getComponent<Position>(entity);

        assert(position.x == 10.0f);
        assert(position.y == 20.0f);

        printPassed("CommandBuffer::remove component");
    }

    void CommandBufferTest::testPlaybackOrder()
    {
        World world;
        CommandBuffer commands;

        Entity entity = world.createEntity();

        commands.addComponent(entity, Position{10.0f, 20.0f});
        commands.removeComponent<Position>(entity);
        commands.addComponent(entity, Position{30.0f, 40.0f});

        assert(commands.size() == 3);

        commands.playback(world);

        // If commands execute in order, the final Position
        // must be the one from the third command.
        const Position& position = world.getComponent<Position>(entity);

        assert(position.x == 30.0f);
        assert(position.y == 40.0f);

        assert(commands.empty());

        printPassed("CommandBuffer::playback order");
    }

    void CommandBufferTest::testClear()
    {
        World world;
        CommandBuffer commands;

        Entity entity = world.createEntity();

        commands.addComponent(entity, Position{10.0f, 20.0f});
        commands.destroyEntity(entity);

        assert(commands.size() == 2);
        assert(!commands.empty());

        commands.clear();

        assert(commands.empty());
        assert(commands.size() == 0);

        // Clearing must discard the commands without executing them.
        assert(world.isValid(entity));
        assert(world.tryGetComponent<Position>(entity) == nullptr);

        printPassed("CommandBuffer::clear");
    }

    void CommandBufferTest::testEmpty()
    {
        CommandBuffer commands;

        assert(commands.empty());
        assert(commands.size() == 0);

        printPassed("CommandBuffer::empty");
    }
}