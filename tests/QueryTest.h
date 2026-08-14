#pragma once

namespace tinyecs::test
{
    class QueryTest
    {
    public:
        static void run();

    private:
        static void testIterate();
        static void testEmpty();
        static void testMultipleChunks();
        static void testSkipEmptyArchetype();
    };
}