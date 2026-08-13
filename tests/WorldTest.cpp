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
        testQuery();
    }

    void WorldTest::testCreateEntity()
    {
        World world;

        Entity first = world.createEntity();
        Entity second = world.createEntity();
        Entity third = world.createEntity();

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

    void WorldTest::testDestroyEntity()
    {
        World world;

        Entity first = world.createEntity();
        Entity second = world.createEntity();
        Entity third = world.createEntity();

        assert(world.isValid(first));
        assert(world.isValid(second));
        assert(world.isValid(third));

        world.destroyEntity(second);

        assert(!world.isValid(second));
        assert(world.isValid(first));
        assert(world.isValid(third));

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

    void WorldTest::testQuery()
    {
        World world;

        Entity positionOnly = world.createEntity();
        Entity positionVelocity = world.createEntity();
        Entity velocityOnly = world.createEntity();

        world.addComponent(positionOnly, Position{10.0f, 20.0f});

        world.addComponent(positionVelocity, Position{30.0f, 40.0f});
        world.addComponent(positionVelocity, Velocity{3.0f, 4.0f});

        world.addComponent(velocityOnly, Velocity{5.0f, 6.0f});

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

        printPassed("World::query");
    }
}