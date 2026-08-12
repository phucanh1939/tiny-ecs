#include "WorldTest.h"

#include <cassert>

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

    void WorldTest::run()
    {
        testCreateEntity();
        testDestroyEntity();
        testAddAndGetComponent();
        testRemoveComponent();
    }

    void WorldTest::testCreateEntity()
    {
        World world;

        Entity first = world.createEntity();
        Entity second = world.createEntity();
        Entity third = world.createEntity();

        // Entities should be valid.
        assert(first.isValid());
        assert(second.isValid());
        assert(third.isValid());

        // IDs should be unique and sequential for new entities.
        assert(first.id == 0);
        assert(second.id == 1);
        assert(third.id == 2);

        // Newly created entities should have the initial version.
        assert(first.version == 1);
        assert(second.version == 1);
        assert(third.version == 1);

        // World should recognize the created entities as valid.
        assert(world.isValid(first));
        assert(world.isValid(second));
        assert(world.isValid(third));

        printPassed("World::create entity");
    }

    void WorldTest::testDestroyEntity()
    {
        World world;

        Entity first = world.createEntity();
        Entity second = world.createEntity();
        Entity third = world.createEntity();

        assert(world.isValid(first));
        assert(world.isValid(second));
        assert(world.isValid(third));

        // Destroy the middle entity.
        world.destroyEntity(second);

        // Destroyed entity is no longer valid.
        assert(!world.isValid(second));

        // Other entities remain valid.
        assert(world.isValid(first));
        assert(world.isValid(third));

        // The ID should be reusable.
        Entity reused = world.createEntity();

        assert(reused.id == second.id);
        assert(reused.version != second.version);
        assert(world.isValid(reused));

        printPassed("World::destroy entity");
    }

    void WorldTest::testAddAndGetComponent()
    {
        World world;
        Entity entity = world.createEntity();
        Position position{10.0f, 20.0f};

        world.addComponent(entity, position);

        const Position &result = world.getComponent<Position>(entity);
        assert(result.x == 10.0f);
        assert(result.y == 20.0f);
        assert(&result == &world.getComponent<Position>(entity));

        Position *tryResult = world.tryGetComponent<Position>(entity);
        assert(tryResult != nullptr);
        assert(tryResult == &world.getComponent<Position>(entity));

        assert(world.tryGetComponent<Velocity>(entity) == nullptr);

        printPassed("World::add and get component");
    }

    void WorldTest::testRemoveComponent()
    {
        World world;
        Entity entity = world.createEntity();
        Position position{10.0f, 20.0f};
        Velocity velocity{1.0f, 2.0f};

        world.addComponent(entity, position);
        world.addComponent(entity, velocity);

        assert(world.tryGetComponent<Position>(entity) != nullptr);
        assert(world.tryGetComponent<Velocity>(entity) != nullptr);

        world.removeComponent<Velocity>(entity);

        const Position &result = world.getComponent<Position>(entity);
        assert(result.x == 10.0f);
        assert(result.y == 20.0f);

        assert(world.tryGetComponent<Position>(entity) != nullptr);
        assert(world.tryGetComponent<Velocity>(entity) == nullptr);
        assert(world.isValid(entity));

        printPassed("World::remove component");
    }
}