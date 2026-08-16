#pragma once

namespace tinyecs::test
{
    class CommandBufferTest
    {
    public:
        static void run();

    private:
        static void testCreateEntity();
        static void testDestroyEntity();
        static void testAddComponent();
        static void testRemoveComponent();
        static void testPlaybackOrder();
        static void testClear();
        static void testEmpty();
    };
}