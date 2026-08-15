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
        testCreateEntityWithComponents();
        testDestroyEntity();
        testAddAndGetComponent();
        testRemoveComponent();
        testQuery();
    }

    void WorldTest::testCreateEntity()
    {
        World world;

        assert(world.entityCount() == 0);

        Entity first = world.createEntity();

        assert(world.entityCount() == 1);

        Entity second = world.createEntity();

        assert(world.entityCount() == 2);

        Entity third = world.createEntity();

        assert(world.entityCount() == 3);

        assert(first.isValid());
        assert(second.isValid());
        assert(third.isValid());

        assert(first.id == 0);
        assert(second.id == 1);
        assert(third.id == 2);

        assert(first.version == 1);
        assert(second.version == 1);
        assert(third.version == 1);

        assert(world.isValid(first));
        assert(world.isValid(second));
        assert(world.isValid(third));

        printPassed("World::create entity");
    }

    void WorldTest::testCreateEntityWithComponents()
    {
        World world;

        assert(world.entityCount() == 0);

        Entity entity = world.createEntity(
            Position{10.0f, 20.0f},
            Velocity{1.0f, 2.0f},
            Health{100});

        assert(world.entityCount() == 1);
        assert(world.isValid(entity));

        const Position &position = world.getComponent<Position>(entity);
        const Velocity &velocity = world.getComponent<Velocity>(entity);
        const Health &health = world.getComponent<Health>(entity);

        assert(position.x == 10.0f);
        assert(position.y == 20.0f);

        assert(velocity.x == 1.0f);
        assert(velocity.y == 2.0f);

        assert(health.value == 100);

        printPassed("World::create entity with components");
    }

    void WorldTest::testDestroyEntity()
    {
        World world;

        Entity first = world.createEntity();
        Entity second = world.createEntity();
        Entity third = world.createEntity();

        assert(world.entityCount() == 3);

        assert(world.isValid(first));
        assert(world.isValid(second));
        assert(world.isValid(third));

        world.destroyEntity(second);

        assert(world.entityCount() == 2);

        assert(!world.isValid(second));
        assert(world.isValid(first));
        assert(world.isValid(third));

        Entity reused = world.createEntity();

        // Reusing an ID still creates one live entity.
        assert(world.entityCount() == 3);

        assert(reused.id == second.id);
        assert(reused.version != second.version);
        assert(world.isValid(reused));

        printPassed("World::destroy entity");
    }

    void WorldTest::testAddAndGetComponent()
    {
        World world;
        Entity entity = world.createEntity();

        assert(world.entityCount() == 1);

        Position position{10.0f, 20.0f};

        world.addComponent(entity, position);

        // Adding components doesn't create another entity.
        assert(world.entityCount() == 1);

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

        assert(world.entityCount() == 1);

        assert(world.tryGetComponent<Position>(entity) != nullptr);
        assert(world.tryGetComponent<Velocity>(entity) != nullptr);

        world.removeComponent<Velocity>(entity);

        // Removing a component doesn't destroy the entity.
        assert(world.entityCount() == 1);

        const Position &result = world.getComponent<Position>(entity);

        assert(result.x == 10.0f);
        assert(result.y == 20.0f);

        assert(world.tryGetComponent<Position>(entity) != nullptr);
        assert(world.tryGetComponent<Velocity>(entity) == nullptr);
        assert(world.isValid(entity));

        printPassed("World::remove component");
    }

    void WorldTest::testQuery()
    {
        World world;

        Entity positionOnly = world.createEntity();
        Entity positionVelocity = world.createEntity();
        Entity velocityOnly = world.createEntity();

        assert(world.entityCount() == 3);

        world.addComponent(positionOnly, Position{10.0f, 20.0f});

        world.addComponent(positionVelocity, Position{30.0f, 40.0f});
        world.addComponent(positionVelocity, Velocity{3.0f, 4.0f});

        world.addComponent(velocityOnly, Velocity{5.0f, 6.0f});

        // Querying and adding components don't affect entity count.
        assert(world.entityCount() == 3);

        auto query = world.query<Position, Velocity>();

        std::size_t count = 0;

        for (auto it = query.begin(); it != query.end(); ++it)
        {
            auto [entity, position, velocity] = *it;

            assert(entity == positionVelocity);
            assert(position.x == 30.0f);
            assert(position.y == 40.0f);
            assert(velocity.x == 3.0f);
            assert(velocity.y == 4.0f);

            ++count;
        }

        assert(count == 1);

        count = 0;

        for (auto [entity, position, velocity] : query)
        {
            assert(entity == positionVelocity);
            assert(position.x == 30.0f);
            assert(position.y == 40.0f);
            assert(velocity.x == 3.0f);
            assert(velocity.y == 4.0f);

            ++count;
        }

        assert(count == 1);
        assert(world.entityCount() == 3);

        printPassed("World::query");
    }
}
