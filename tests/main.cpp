#include <cassert>
#include <iostream>

#include "ComponentSignatureTest.h"
#include "ComponentRegistryTest.h"
#include "ComponentColumnTest.h"
#include "ChunkTest.h"
#include "ArchetypeTest.h"
#include "ArchetypeRegistryTest.h"
#include "WorldTest.h"

using namespace tinyecs::test;

int main()
{
    ComponentSignatureTest::run();
    ComponentRegistryTest::run();
    ComponentColumnTest::run();
    ChunkTest::run();
    ArchetypeTest::run();
    ArchetypeRegistryTest::run();
    WorldTest::run();
    std::cout << "\033[32m✓ All tests passed\033[0m\n";
    return 0;
}

