#include <iostream>

#include "TestUtils.h"

namespace tinyecs::test
{
    void printPassed(const char *testName)
    {
        std::cout << "\033[32m✓ " << testName << " passed\033[0m\n";
    }
}
