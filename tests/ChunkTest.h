#pragma once

namespace tinyecs::test
{
    class ChunkTest
    {
    public:
        static void run();

    private:
        static void testEmpty();
        static void testAddEntity();
        static void testFull();
        static void testGetEntity();
        static void testGetComponent();
        static void testRemoveLast();
        static void testRemoveSwapBack();
        static void testCopyEntity();
        static void testCopyEntityToFullChunk();
    };
}
