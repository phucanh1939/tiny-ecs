#include <cassert>
#include <iostream>

#include "ComponentSignatureTest.h"
#include "ComponentRegistryTest.h"
#include "ComponentColumnTest.h"
#include "ChunkTest.h"

using namespace tinyecs::test;

int main()
{
    ComponentSignatureTest::run();
    ComponentRegistryTest::run();
    ComponentColumnTest::run();
    ChunkTest::run();
    std::cout << "\033[32m✓ All tests passed\033[0m\n";
    return 0;
}

