#include "ComponentRegistryTest.h"

#include <cassert>

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
            float z;
        };

        struct Velocity
        {
            float x;
            float y;
            float z;
        };

        struct Health
        {
            float currentHeath;
            float maxHealth;
        };
    }

    void ComponentRegistryTest::run()
    {
        testSameTypeReturnsSameComponentType();
        testDifferentTypesReturnDifferentComponentTypes();
        testGetComponentInfoByType();
        testGetComponentInfoByComponentType();
    }

    void ComponentRegistryTest::testSameTypeReturnsSameComponentType()
    {
        ComponentType first = ComponentRegistry::getComponentType<Position>();
        ComponentType second = ComponentRegistry::getComponentType<Position>();

        assert(first == second);

        printPassed("ComponentRegistry same type");
    }

    void ComponentRegistryTest::testDifferentTypesReturnDifferentComponentTypes()
    {
        ComponentType position = ComponentRegistry::getComponentType<Position>();
        ComponentType velocity = ComponentRegistry::getComponentType<Velocity>();
        ComponentType health = ComponentRegistry::getComponentType<Health>();

        assert(position != velocity);
        assert(position != health);
        assert(velocity != health);

        printPassed("ComponentRegistry different types");
    }

    void ComponentRegistryTest::testGetComponentInfoByType()
    {
        const ComponentInfo &info = ComponentRegistry::getComponentInfo<Position>();

        ComponentType type = ComponentRegistry::getComponentType<Position>();

        assert(info.type == type);
        assert(info.size == sizeof(Position));

        printPassed("ComponentRegistry getComponentInfo<T>");
    }

    void ComponentRegistryTest::testGetComponentInfoByComponentType()
    {
        ComponentType type = ComponentRegistry::getComponentType<Position>();

        const ComponentInfo &info = ComponentRegistry::getComponentInfo(type);

        assert(info.type == type);

        printPassed("ComponentRegistry getComponentInfo(ComponentType)");
    }
}