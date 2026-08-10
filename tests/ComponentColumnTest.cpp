#include <cassert>
#include <cstring>

#include <tinyecs/ComponentColumn.h>

#include "ComponentColumnTest.h"
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

        ComponentInfo createPositionInfo()
        {
            return ComponentInfo{
                .type = 42,
                .size = sizeof(Position)
            };
        }
    }

    void ComponentColumnTest::run()
    {
        testType();
        testEmpty();
        testAdd();
        testGet();
        testConstGet();
        testRemove();
    }

    void ComponentColumnTest::testType()
    {
        ComponentColumn column(createPositionInfo());

        assert(column.type() == 42);

        printPassed("ComponentColumn::type");
    }

    void ComponentColumnTest::testEmpty()
    {
        ComponentColumn column(createPositionInfo());

        assert(column.count() == 0);

        printPassed("ComponentColumn empty");
    }

    void ComponentColumnTest::testAdd()
    {
        ComponentColumn column(createPositionInfo());

        std::uint32_t first = column.add();
        assert(first == 0);
        assert(column.count() == 1);

        std::uint32_t second = column.add();
        assert(second == 1);
        assert(column.count() == 2);

        std::uint32_t third = column.add();
        assert(third == 2);
        assert(column.count() == 3);

        printPassed("ComponentColumn::add");
    }

    void ComponentColumnTest::testGet()
    {
        ComponentColumn column(createPositionInfo());

        column.add();
        column.add();

        Position first{10.0f, 20.0f};
        Position second{30.0f, 40.0f};

        std::memcpy(column.get(0), &first, sizeof(Position));
        std::memcpy(column.get(1), &second, sizeof(Position));

        auto *firstResult = static_cast<Position *>(column.get(0));

        auto *secondResult =static_cast<Position *>(column.get(1));

        assert(firstResult->x == 10.0f);
        assert(firstResult->y == 20.0f);

        assert(secondResult->x == 30.0f);
        assert(secondResult->y == 40.0f);

        printPassed("ComponentColumn::get");
    }

    void ComponentColumnTest::testConstGet()
    {
        ComponentColumn column(createPositionInfo());

        column.add();

        Position position{10.0f, 20.0f};

        std::memcpy(column.get(0), &position, sizeof(Position));

        const ComponentColumn &constColumn = column;

        const auto *result = static_cast<const Position *>(constColumn.get(0));

        assert(result->x == 10.0f);
        assert(result->y == 20.0f);

        printPassed("ComponentColumn::get const");
    }

    void ComponentColumnTest::testRemove()
    {
        ComponentColumn column(createPositionInfo());

        column.add();
        column.add();
        column.add();

        Position first{1.0f, 1.0f};
        Position second{2.0f, 2.0f};
        Position third{3.0f, 3.0f};

        std::memcpy(column.get(0), &first, sizeof(Position));
        std::memcpy(column.get(1), &second, sizeof(Position));
        std::memcpy(column.get(2), &third, sizeof(Position));

        // Remove the middle element.
        // Expected: [first, third]
        column.remove(1);

        assert(column.count() == 2);

        auto *result0 =static_cast<Position *>(column.get(0));

        auto *result1 =static_cast<Position *>(column.get(1));

        assert(result0->x == 1.0f);
        assert(result0->y == 1.0f);

        // Last element should have moved into index 1.
        assert(result1->x == 3.0f);
        assert(result1->y == 3.0f);

        printPassed("ComponentColumn::remove");
    }
}