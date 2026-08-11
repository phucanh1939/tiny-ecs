#pragma once

namespace tinyecs::test
{
    class ArchetypeRegistryTest
    {
    public:
        static void run();

    private:
        static void testCreate();
        static void testRoot();
        static void testFind();
        static void testGetOrCreate();
    };
}
