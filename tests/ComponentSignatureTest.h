#pragma once

namespace tinyecs::test
{
    class ComponentSignatureTest
    {
    public:
        static void run();

    private:
        static void testAdd();
        static void testRemove();
        static void testEmpty();
        static void testEquality();
        static void testIterator();
        static void testRangeBasedFor();
        static void testEmptyIterator();
    };
}