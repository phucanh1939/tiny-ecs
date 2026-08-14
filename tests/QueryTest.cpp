#include "QueryTest.h"

#include <cassert>
#include <vector>

#include <tinyecs/Archetype.h>
#include <tinyecs/ComponentRegistry.h>
#include <tinyecs/Query.h>

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
    }

    void QueryTest::run()
    {
        testIterate();
        testEmpty();
        testMultipleChunks();
        testSkipEmptyArchetype();
    }

    void QueryTest::testIterate()
    {
        const ComponentType positionType = ComponentRegistry::getComponentType<Position>();
        const ComponentType velocityType = ComponentRegistry::getComponentType<Velocity>();

        ComponentSignature signature;
        signature.add(positionType);
        signature.add(velocityType);

        Archetype archetype(signature);

        Entity first{1, 1};
        Entity second{2, 1};
        Entity third{3, 1};

        EntityLocation firstLocation = archetype.addEntity(first);
        EntityLocation secondLocation = archetype.addEntity(second);
        EntityLocation thirdLocation = archetype.addEntity(third);

        auto *firstPosition = static_cast<Position *>(archetype.getComponent(positionType, firstLocation));
        auto *firstVelocity = static_cast<Velocity *>(archetype.getComponent(velocityType, firstLocation));

        auto *secondPosition = static_cast<Position *>(archetype.getComponent(positionType, secondLocation));
        auto *secondVelocity = static_cast<Velocity *>(archetype.getComponent(velocityType, secondLocation));

        auto *thirdPosition = static_cast<Position *>(archetype.getComponent(positionType, thirdLocation));
        auto *thirdVelocity = static_cast<Velocity *>(archetype.getComponent(velocityType, thirdLocation));

        *firstPosition = {10.0f, 20.0f};
        *firstVelocity = {1.0f, 2.0f};

        *secondPosition = {30.0f, 40.0f};
        *secondVelocity = {3.0f, 4.0f};

        *thirdPosition = {50.0f, 60.0f};
        *thirdVelocity = {5.0f, 6.0f};

        Query<Position, Velocity> query({&archetype});

        std::size_t count = 0;

        for (auto it = query.begin(); it != query.end(); ++it)
        {
            auto [entity, position, velocity] = *it;

            if (count == 0)
            {
                assert(entity == first);
                assert(position.x == 10.0f);
                assert(position.y == 20.0f);
                assert(velocity.x == 1.0f);
                assert(velocity.y == 2.0f);
            }
            else if (count == 1)
            {
                assert(entity == second);
                assert(position.x == 30.0f);
                assert(position.y == 40.0f);
                assert(velocity.x == 3.0f);
                assert(velocity.y == 4.0f);
            }
            else if (count == 2)
            {
                assert(entity == third);
                assert(position.x == 50.0f);
                assert(position.y == 60.0f);
                assert(velocity.x == 5.0f);
                assert(velocity.y == 6.0f);
            }

            ++count;
        }

        assert(count == 3);

        printPassed("Query::iterate");
    }

    void QueryTest::testEmpty()
    {
        ComponentSignature signature;
        Archetype archetype(signature);

        Query<> query({&archetype});

        assert(query.begin() == query.end());

        printPassed("Query::empty");
    }

    void QueryTest::testMultipleChunks()
    {
        ComponentSignature signature;
        Archetype archetype(signature);

        // Fill full a chunk and next 10 items to the 2nd chunk
        std::uint32_t enittyCount = archetype.chunkCapacity() + 10;
        for (std::uint32_t i = 0; i < enittyCount; ++i)
        {
            archetype.addEntity(Entity{i, 1});
        }

        Query<> query({&archetype});

        std::size_t count = 0;

        for (auto it = query.begin(); it != query.end(); ++it)
        {
            auto [entity] = *it;
            assert(entity.id == count);
            ++count;
        }

        assert(count == enittyCount);
        assert(archetype.chunkCount() == 2);

        printPassed("Query::iterate multiple chunks");
    }

    void QueryTest::testSkipEmptyArchetype()
    {
        ComponentSignature signature;

        Archetype first(signature);
        Archetype empty(signature);
        Archetype last(signature);

        Entity firstEntity{1, 1};
        Entity lastEntity{2, 1};

        first.addEntity(firstEntity);
        last.addEntity(lastEntity);

        Query<> query({&first, &empty, &last});

        std::vector<Entity> result;

        for (auto it = query.begin(); it != query.end(); ++it)
        {
            auto [entity] = *it;
            result.push_back(entity);
        }

        assert(result.size() == 2);
        assert(result[0] == firstEntity);
        assert(result[1] == lastEntity);

        printPassed("Query::skip empty archetype");
    }
}
