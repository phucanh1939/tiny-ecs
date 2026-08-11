#pragma once

namespace tinyecs::test
{
    class ArchetypeTest
    {
    public:
        static void run();

    private:
        static void testAddEntity();
        static void testRemoveEntityLast();
        static void testRemoveSwapback();
        static void testCopyEntity();
        static void testGetEntity();
        static void testGetComponent();
        static void testInvalidComponent();
    };
}
