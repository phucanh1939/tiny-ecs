#pragma once

namespace tinyecs::test
{
    class ComponentColumnTest
    {
    public:
        static void run();

    private:
        static void testType();
        static void testEmpty();
        static void testAdd();
        static void testGet();
        static void testConstGet();
        static void testRemove();
    };
}