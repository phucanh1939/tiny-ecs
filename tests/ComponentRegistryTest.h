#pragma once

namespace tinyecs::test
{
class ComponentRegistryTest
{
public:
    static void run();

private:
    static void testSameTypeReturnsSameComponentType();
    static void testDifferentTypesReturnDifferentComponentTypes();
    static void testGetComponentInfoByType();
    static void testGetComponentInfoByComponentType();
};
}