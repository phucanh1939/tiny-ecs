#pragma once

namespace tinyecs::test
{
    class WorldTest
    {
    public:
        static void run();

    private:
        static void testCreateEntity();
        static void testCreateEntityWithComponents();
        static void testDestroyEntity();
        static void testAddAndGetComponent();
        static void testRemoveComponent();
        static void testQuery();
    };
}