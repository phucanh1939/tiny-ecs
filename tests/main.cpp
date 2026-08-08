#include <cassert>
#include <iostream>

#include "ComponentSignatureTest.h"

using namespace tinyecs::test;

int main()
{
    ComponentSignatureTest::run();
    std::cout << "\033[32m✓ All tests passed\033[0m\n";
    return 0;
}

